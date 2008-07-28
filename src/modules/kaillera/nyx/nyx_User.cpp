/******************************************************************************
O                                                           8d8b. Yb  dP Yb dP 
O                                                           8P Y8  YbdP   `8.  
Open Kaillera: Kaillera Server                              8   8   dP   dP Yb 
*******************************************************************dP**********
Copyright (c) Open Kaillera Team 2003-2008

Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal in
the Software without restriction, including without limitation the rights to
use, copy, modify, merge, publish, distribute, and/or sell copies of the
Software, and to permit persons to whom the Software is furnished to do so,
subject to the following conditions:

The above copyright notice and this permission notice must be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
******************************************************************************/

#include "nyx.h"

User::User(unsigned short uid) : state(CONNECTING)
{
	*nick = 0;
	*app = 0;
	connection = 0;
	lastDataArrival = serverTime.getFixedTime();
	id = uid;

	ping = gamePlayerIndex = gamePlayerDelay = 0;
	lastDataSent = 0;

	gamePlayerReady = false;

	game = 0;

	inputLength = 0;
}

User::~User() {
	TRACE();
	
	int len = inCache.itemsCount() - 1;
	while (len >= 0){
		delete inCache[len--];
	}
	len = outCache.itemsCount() - 1;
	while (len >= 0){
		delete outCache[len--];
	}
}

void User::instructionArrivalCallback(Instruction & ki)
{
	switch (state) {
	case CONNECTING:
		switch (ki.type) {
		case USERLOGN:
			if (gamePlayerIndex == 0) {
				// first login request packet
				lastDataArrival = serverTime.getFixedTime();
				gamePlayerIndex = 1;

				// get user info
				strncpy(nick, ki.user, 31);
				ki.readString(app, 128);
				connection = ki.readSignedInt8();

				if (connection <= config::minConset && connection >= config::maxConset) {
					Instruction ping (SERVPING);
					ping.writeBytes(nick, 16);

					includeInstruction(ping);
					sendMessage();

					lastDataSent = serverTime.getFixedTime();
				} else {
					if (connection >= config::maxConset) {
						sprintf(app, "Rejected: Connection < min allowed");
						Instruction stat(LOGNSTAT);
						stat.writeSignedInt16(id);
						stat.writeString(app);
						includeInstruction(stat);
						sendMessage();
					} else {
						sprintf(app, "Rejected: Connection > max allowed");
						Instruction stat(LOGNSTAT);
						stat.writeSignedInt16(id);
						stat.writeString(app);
						includeInstruction(stat);
						sendMessage();
					}
				}
			}
			break;
		case USERPONG:
			if (gamePlayerIndex > 0) {
				if (gamePlayerIndex < PING_LIMIT) {
					lastDataArrival = serverTime.getFixedTime();
					int _ping = (serverTime.getFixedTime() - lastDataSent) / gamePlayerIndex;
					_ping = ping - (_ping >> PING_CONSIDERATION_RATIO);

					if (_ping <= config::maxPing) {
						Instruction ping (SERVPING);
						ping.writeBytes(nick, 16);

						includeInstruction(ping);
						sendMessage();
						gamePlayerIndex++;
					} else {
						LOG(User ping too high %i (%s) %i, id, nick, _ping);
						sprintf(app, "Rejected: Ping too high (%ims>%ims)", _ping, config::maxPing);
						Instruction stat(LOGNSTAT);
						stat.writeSignedInt16(id);
						stat.writeString(app);
						includeInstruction(stat);
						sendMessage();
						state = DISCONNECTED;
					}
				} else if (gamePlayerIndex == PING_LIMIT) {
					ping = (serverTime.getFixedTime() - lastDataSent) / gamePlayerIndex;

					if (ping <= config::maxPing) {
						lastDataArrival = serverTime.getFixedTime();
						
						gamePlayerDelay = (ping * 60 / connection / 1000) + 1;

						// send login success / server state
						{
							Instruction logn(LONGSUCC);

							logn.writeSignedInt32(UsersList::getCount() - UsersList::getConnectingCount());
							logn.writeSignedInt32(GamesList::getCount());
							UsersList::writeUsersState(logn);
							GamesList::writeGamesState(logn);

							includeInstruction(logn);

							sendMessage();
						}

						state = IDLE;

						// send user joining notification
						{
							Instruction joined (USERJOIN);
							joined.writeUnsignedInt16(id);
							joined.writeSignedInt32(ping);
							joined.writeSignedInt8(connection);
							strcpy(joined.user, nick);
							UsersList::sendToAll(joined);
						}

						if (config::motd.itemsCount() > 0) {
							for (int x = 0; x < config::motd.itemsCount(); x++) {
								Instruction motd(MOTDLINE);
								strcpy(motd.user, "Server");
								motd.writeString(config::motd[x]);
								includeInstruction(motd);								
							}
						}

						sendMessage();
					} else {
						sprintf(app, "Rejected: Ping too high (%ims>%ims)", ping, config::maxPing);
						Instruction stat(LOGNSTAT);
						stat.writeSignedInt16(id);
						stat.writeString(app);
						includeInstruction(stat);
						sendMessage();
						LOG(%s, app);
						state = DISCONNECTED;
					}
				}
			} else {
				// PINGHAX
			}
			break;
		case USERLEAV:
			LOG(User left %i (%s), id, nick);
			state = DISCONNECTED;
			break;
		default:
			state = DISCONNECTED;
			ki.log();
			break;
		};
		break;
	case IDLE:
		lastDataArrival = serverTime.getFixedTime();
		switch (ki.type) {
		case USERPONG:
			
			break;
		case USERLEAV:
			LOG(User left %i (%s), id, nick);
			ki.seek(strlen(ki.getCurrentStringPtr())+1);
			strcpy(ki.user, nick);
			UsersList::sendToAll(ki);
			state = DISCONNECTED;
			leaveGame();
			break;
		case PARTCHAT:
			{
				register int len = strlen(ki.getCurrentStringPtr());

				if (len > 0) {

					register unsigned int ttime = serverTime.getFixedTime() - config::floodMsgTime;

					while (chatTimes.itemsCount() > 0 && ttime > chatTimes[0])
						chatTimes.removeIndex(0);

					chatTimes.addItem(serverTime.getFixedTime());

					if (chatTimes.itemsCount() > config::floodMsgNo) {
						
						{
							Instruction left (USERLEAV);
							strcpy(left.user, nick);
							left.writeString("Washed away by the floods of tsunami");
							UsersList::sendToAll(left);
							LOG(%s (%i) was washed away by the floods of tsunami, nick, id);
						}

						state = DISCONNECTED;
					} else {
						ki.seek(len + 1);
						strcpy(ki.user, nick);
						UsersList::sendToAll(ki);
					}
				}
			}
			break;
		case GAMEMAKE:
			{
				Game * g = new Game(ki.getCurrentStringPtr(), this);
				GamesList::addGame(g);
				game = g;
				
				Instruction gmade (GAMEMAKE);
				gmade.writeString(ki.getCurrentStringPtr());
				gmade.writeString(app);
				gmade.writeSignedInt32(g->id);
				strcpy(gmade.user, nick);
				UsersList::sendToAll(gmade);
				
				g->addUser(this);

				break;
			}
		case GAMRJOIN:
			{
				if (game == 0) {
					unsigned int tid = ki.readUnsignedInt32();
					Game * g = GamesList::findGame(tid);
					if (g != 0 && g->state == Game::WAITING) {
						game = g;
						{
							Instruction gs(GAMRSLST);
							g->writeGameState(gs);
							includeInstruction(gs);
							sendMessage();
						}
						g->addUser(this);
						g->updateStatus();
					}
				}
				break;
			}

		case GAMRLEAV:
			{
				leaveGame();
				break;
			}
		case GAMECHAT:
			if (game != 0) {
				strcpy(ki.user, nick);
				ki.skipString();
				reinterpret_cast<Game*>(game)->sendToAll(ki);
			}
			break;
		case TMOUTRST:
			break;
		default:
			LOGS(Unhandled instruction);
			log();
			ki.log();
			break;
		};
		break;
	case PLAYING:
		ki.log();
		break;
	case DISCONNECTED:
		ki.log();
		break;
	};
}

void User::leaveGame()
{
	if (game != 0) {
		Game * g = reinterpret_cast<Game*>(game);

		if (g->removeUser(this)) {
			GamesList::removeGame(g);
		}

		game = 0;
	}
	//login_timeout = userslist.time_ + 120000;
}

bool User::idleStep() {
	switch (state) {
		case CONNECTING:
			if (serverTime.getFixedTime() - lastDataArrival > CONNECTING_TIMEOUT_LIMIT) {
				LOG(%s timed out while connecting (id=%i), nick, id);
				state = DISCONNECTED;
				return true;
			}
			break;
		case IDLE:
			if (serverTime.getFixedTime() - lastDataArrival > CONNECTION_TIMEOUT) {
				LOG(%s timed out (id=%i), nick, id);

				Instruction timedout(USERLEAV);
				timedout.writeUnsignedInt16(id);
				timedout.writeString("connection timed out");
				strcpy(timedout.user, nick);
				UsersList::sendToAll(timedout);
				
				state = DISCONNECTED;
				return true;
			}
			// check for timeout
			break;
		case PLAYING:


			//			if (game != 0 && gameslist.posof(game) != 0){
			//	if (game->status == 1) {
			//		if (player_ready == 0) {
			//			if (time_ > netsync_timeout_time) {
			//				kprintf("%s client netsync timeout #%i", username, netsync_timeout_count);
			//				sock->resend_message(throughput + 4);
			//				netsync_timeout_time = userslist.time_ + 10000;
			//				if (++netsync_timeout_count==24) {
			//					kprintf("%s client got too many netsync timeouts. leaving game",  username);
			//					drop_game();
			//				}
			//			}
			//		}
			//	} else if (game->status == 2) {
			//		if (time_ > data_timeout_time) {
			//			kprintf("%s client timeout #%i", username, data_timeout_count);
			//			sock->resend_message(throughput + 4);
			//			data_timeout_time = time_ + 2000;
			//			if (++data_timeout_count >= 3) {
			//				kprintf("%s client got too many timeouts. leaving game", username);
			//				drop_game();
			//			}
			//		}
			//	}
			//} else {
			//	leave_game();
			//}



			// retransmissions
			break;
		case DISCONNECTED:
			return true;
	};
	return false;
}

//void k_user::send_data(char * frame_buf, int frame_len){
//	char  tmp_buffer[1024];
//	if (frame_len > 0) {
//		another_working_frame.put_data(frame_buf, frame_len);
//	}
//	int reqlen = connection * frame_len;
//	if (another_working_frame.pos >= reqlen) {
//		if (outgoing_cache.length > 0) {
//			for (int i = 0; i > outgoing_cache.length; i++) {
//				outgoing_cache.get(i)->peek_data(tmp_buffer, another_working_frame.pos);
//				if (memcmp(tmp_buffer, another_working_frame.buffer, another_working_frame.pos) == 0) {
//					k_instruction kix;
//					kix.type = INSTRUCTION_GAMCDATA;
//					kix.store_char(another_working_frame.pos);
//					sock->send_instruction(&kix);
//					another_working_frame.pos = 0;
//					return;
//				}
//			}
//		}
//		another_working_frame.peek_data(tmp_buffer, reqlen);
//		k_instruction kix;
//		kix.type = INSTRUCTION_GAMEDATA;
//		kix.store_short(another_working_frame.pos);
//		kix.store_bytes(tmp_buffer, reqlen);
//		sock->send_instruction(&kix);
//		if (outgoing_cache.length == 0x100) {
//			delete outgoing_cache.get(0);
//			outgoing_cache.remove(0);
//		}
//		k_frame * kfx = new k_frame(another_working_frame.buffer, another_working_frame.pos);
//		outgoing_cache.add(kfx);
//		another_working_frame.pos = 0;
//	}
//}
