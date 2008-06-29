/******************************************************************************
          .d8888b.   .d8888b.  
         d88P  Y88b d88P  Y88b 
         888    888        888 
88888b.  888    888      .d88P 
888 "88b 888    888  .od888P"  
888  888 888    888 d88P"      
888  888 Y88b  d88P 888"       
888  888  "Y8888P"  888888888              Open Kaillera Arcade Netplay Library
*******************************************************************************
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
#include "ConfigurationManager.h"
#include "StringUtils.h"

#define USE_WINDOWS_INI
//#define USE_CPP_CUSTOM_INI

#ifdef USE_WINDOWS_INI
#include <windows.h>
#endif

#ifdef USE_CPP_CUSTOM_INI
#include <fstream>
using namespace std;
#endif

namespace n02 {

	void ConfigurationManager::loadFromFile(TCHAR * fileName, TCHAR * module)
	{
#ifdef USE_WINDOWS_INI
		TCHAR file[2048];
		if (_tcschr(fileName, '\\')==0) {
			GetCurrentDirectory(2048, file);
			_tcscat(file, _T("\\"));
			_tcscat(file, fileName);

			OFSTRUCT of;

			char fileT[2048];
			StringUtils::TCHARToUTF8(reinterpret_cast<unsigned char*>(fileT), file);

			if (OpenFile(fileT, &of, OF_EXIST) == HFILE_ERROR) {
				_tcscpy(file, fileName);
			}
		} else {
			_tcscpy(file, fileName);
		}
		for (int x = 0; x < itemsCount(); x++) {
			ConfigurationItem * cnf = getItemPtr(x);
			switch (cnf->type) {
				case ConfigurationItem::INT:
					*reinterpret_cast<unsigned int*>(cnf->pointer) = GetPrivateProfileInt(module, cnf->name, reinterpret_cast<int>(cnf->defaultValuePointer), file);
					break;
				case ConfigurationItem::STRING:
					GetPrivateProfileString(module, cnf->name, reinterpret_cast<LPCTSTR>(cnf->defaultValuePointer), reinterpret_cast<LPTSTR>(cnf->pointer), cnf->length, file);
					break;
				case ConfigurationItem::STRUCT:
					{
						if (cnf->length < 256) {
							TCHAR bufferL[520];
							bufferL[0] = 0;
							GetPrivateProfileString(module, cnf->name, _T(""), bufferL, 520, file);
							int slen = _tcslen(bufferL) >> 1;
							if (slen >= cnf->length) {
								StringUtils::strToBytesT(reinterpret_cast<unsigned char*>(cnf->pointer), bufferL, cnf->length);
							}
						}
					}
					break;
				case ConfigurationItem::STRLIST:
					{
						DynamicOrderedArray<TCHAR*> * list = (DynamicOrderedArray<TCHAR*>*)cnf->pointer;
						while (list->itemsCount() > 0) {
							delete list->getItem(itemsCount()-1);
							list->removeIndex(itemsCount()-1);
						}
						TCHAR key[44];
						_tcscpy(key, cnf->name);
						_tcscat(key, _T("_count"));
						int count = GetPrivateProfileInt(module, key, 0, file);
						for (int y = 0; y < count; y++) {
							TCHAR * newValue = new TCHAR[cnf->length];
							_tcscpy(key, cnf->name);
							_tcscat(key, _T("_"));
							_tcscat(key, StringUtils::intToAlphaT(y));
							GetPrivateProfileString(module, key, _T(""), newValue, cnf->length, file);
							list->addItem(newValue);
						}
					}
					break;
			};
		}
#endif
	// TODO: Add c++ file version of setting loading
	}

	void ConfigurationManager::saveToFile(TCHAR * fileName, TCHAR * module)
	{
#ifdef USE_WINDOWS_INI
		TCHAR file[2048];
		if (_tcschr(fileName, '\\')==0) {
			char fileT[2048];
			OFSTRUCT of;
			GetCurrentDirectory(2048, file);
			_tcscat(file, _T("\\"));
			_tcscat(file, fileName);
			StringUtils::TCHARToUTF8(reinterpret_cast<unsigned char*>(fileT), file);
			if (OpenFile(fileT, &of, OF_EXIST) == HFILE_ERROR) {
				_tcscpy(file, fileName);
			}
		} else {
			_tcscpy(file, fileName);
		}
		for (int x = 0; x < itemsCount(); x++) {
			ConfigurationItem * cnf = getItemPtr(x);
			switch (cnf->type) {
				case ConfigurationItem::INT:
					{
						WritePrivateProfileString(module, cnf->name, StringUtils::uintToAlphaT(*reinterpret_cast<unsigned int*>(cnf->pointer)), file);
					}
					break;
				case ConfigurationItem::STRING:
					WritePrivateProfileString(module, cnf->name, (LPTSTR)cnf->pointer, file);
					break;
				case ConfigurationItem::STRUCT:
					{
						if (cnf->length < 256) {
							TCHAR bufferL[520];
							bufferL[0] = 0;
							StringUtils::bytesToStrT(bufferL, reinterpret_cast<unsigned char*>(cnf->pointer), cnf->length, 512);
							WritePrivateProfileString(module, cnf->name, bufferL, file);
						}
					}
					break;
				case ConfigurationItem::STRLIST:
					{
						DynamicOrderedArray<TCHAR*> * list = reinterpret_cast<DynamicOrderedArray<TCHAR*>*>(cnf->pointer);
						TCHAR key[44];
						_tcscpy(key, cnf->name);
						_tcscat(key, _T("_count"));
						WritePrivateProfileString(module, key, StringUtils::intToAlphaT(list->itemsCount()), file);
						for (int y = 0; y < list->itemsCount(); y++) {
							_tcscpy(key, cnf->name);
							_tcscat(key, _T("_"));
							_tcscat(key, StringUtils::intToAlphaT(y));
							WritePrivateProfileString(module, key, list->getItem(y), file);
						}
					}
					break;
			};
		}
#endif

#ifdef USE_CPP_CUSTOM_INI
		ofstream of;
		of.open(fileName);

		for (int x = 0; x < itemsCount(); x++) {
			ConfigurationItem * cnf = getItemPtr(x);
			switch (cnf->type) {
				case ConfigurationItem::INT:
					{
						char key[1024];
						StringUtils::TCHARToUTF8(reinterpret_cast<unsigned char*>(key), cnf->name);
						strcat(key,"=");
						StringUtils::uintToAlpha(key + strlen(key), *reinterpret_cast<unsigned int*>(cnf->pointer));
						strcat(key,"\r\n");
						of.write(key, strlen(key));
					}
					break;
				case ConfigurationItem::STRING:
					{
						char key[1024];
						StringUtils::TCHARToUTF8(reinterpret_cast<unsigned char*>(key), cnf->name);
						strcat(key,"=");
						StringUtils::TCHARToUTF8(reinterpret_cast<unsigned char*>(key + strlen(key)), reinterpret_cast<TCHAR*>(cnf->pointer));
						strcat(key,"\r\n");
						of.write(key, strlen(key));
					}
					break;
				case ConfigurationItem::STRUCT:
					{
						if (cnf->length < 256) {
							char key[1024];
							StringUtils::TCHARToUTF8(reinterpret_cast<unsigned char*>(key), cnf->name);
							strcat(key,"=");
							StringUtils::bytesToStr(key + strlen(key), reinterpret_cast<unsigned char*>(cnf->pointer), cnf->length, 512);
							strcat(key,"\r\n");
							of.write(key, strlen(key));
						}
					}
					break;
				case ConfigurationItem::STRLIST:
					{
						DynamicOrderedArray<TCHAR*> * list = ((DynamicOrderedArray<TCHAR*>*)cnf->pointer);
						char ikey[44];
						StringUtils::TCHARToUTF8(reinterpret_cast<unsigned char*>(ikey), cnf->name);
						for (int y = 0; y < list->itemsCount(); y++) {
							char key[1024];
							strcpy(key, ikey);
							strcat(key,"=");
							StringUtils::TCHARToUTF8(reinterpret_cast<unsigned char*>(key + strlen(key)), list->getItem(y));
							strcat(key,"\r\n");
							of.write(key, strlen(key));
						}
					}
					break;
			};
		}

		of.close();
#endif
	}

	void ConfigurationManager::save(TCHAR * modName)
	{
		saveToFile(_T("n02.ini"), modName);
	}

	void ConfigurationManager::load(TCHAR * modName)
	{
		loadFromFile(_T("n02.ini"), modName);
	}

	ConfigurationManager::~ConfigurationManager()
	{
		for (int x = 0; x < itemsCount(); x++) {
			ConfigurationItem * cnf = getItemPtr(x);
			if (cnf->type == ConfigurationItem::STRLIST) {
				DynamicOrderedArray<TCHAR*> * list = reinterpret_cast<DynamicOrderedArray<TCHAR*>*>(cnf->pointer);
				while (list->itemsCount() > 0) {
					delete list->getItem(list->itemsCount()-1);
					list->removeIndex(list->itemsCount()-1);
				}
			}
		}
	}

	ConfigurationManager::ConfigurationManager(ConfigurationItem * configTable)
	{
		useConfgTable(configTable);
	}

	ConfigurationManager::ConfigurationManager(ConfigurationItem * configTable, int len)
	{
		useConfgTable(configTable, len);
	}

	ConfigurationManager::ConfigurationManager() {}

	void ConfigurationManager::add(ConfigurationItem*item)
	{
		for (int x = 0; x < itemsCount(); x++) {
			if (_tcscmp(item->name, getItemPtr(x)->name) < 0) {
				insertItemPtr(item, x);
				return;
			}
		}
		addItemPtr(item);
	}

	void ConfigurationManager::remove(TCHAR * name)
	{
		for (int x = 0; x < itemsCount(); x++) {
			if (_tcscmp(name, getItemPtr(x)->name) == 0) {
				removeIndex(x);						
				break;
			}
		}
	}

	void ConfigurationManager::useConfgTable(ConfigurationItem * configTable)
	{
		int count = 0;
		ConfigurationItem * configPtr = configTable;
		while (configPtr->name[0] != 0) {
			configPtr++;
			count++;
		}
		useConfgTable(configTable, count);
	}

	void ConfigurationManager::useConfgTable(ConfigurationItem * configTable, int len)
	{
		clearItems();
		while (len-->0) {
			add(configTable);
			configTable++;
		}
	}

};
