
project.name = "n02"

project.bindir = "../../../release"
project.libdir = "../../../release"

project.configs = { "Release" }

package = newpackage()
package.name = "n02"
package.kind = "dll"
package.language = "c++"
package.objdir = "../../../temp/"

package.config["Release"].objdir = "../../../temp/"
package.config["Release"].target = "n02"
package.config["Release"].defines       = { "N02_LINUX=1" };
package.includepaths = { 
    "../../common",
    "../../interface",
    "../../juce",
    "../../misc",
    "../../modules",
    "../../common",
    "../../ui"
}

package.files = {
	"../../common/BsdSocket.cpp",
	"../../common/BsdSocket.h",
	"../../common/ConfigurationManager.cpp",
	"../../common/ConfigurationManager.h",
	"../../common/DataQueue.h",
	"../../common/DynamicAllocator.h",
	"../../common/DynamicArray.h",
	"../../common/DynamicBuffer.h",
	"../../common/DynamicOrderedArray.h",
	"../../common/DynamicQueue.h",
	"../../common/DynamicStack.h",
	"../../common/GlobalTimer.h",
	"../../common/LinkedList.h",
	"../../common/ListGenerics.h",
	"../../common/Logger.cpp",
	"../../common/Logger.h",
	"../../common/PosixThread.cpp",
	"../../common/PosixThread.h",
	"../../common/SocketAddress.cpp",
	"../../common/SocketAddress.h",
	"../../common/StaticAllocator.h",
	"../../common/StaticArray.h",
	"../../common/StaticBuffer.h",
	"../../common/StaticOrderedArray.h",
	"../../common/StaticQueue.h",
	"../../common/StaticStack.h",
	"../../common/StringUtils.cpp",
	"../../common/StringUtils.h",
	"../../common/UdpSocket.h",
	"../../common/_common.cpp",
	"../../common/_common.h",
	"../../common/common.cpp",
	"../../common/common.h",
	"../../common/trace.cpp",
	"../../common/trace.h",
	"../../interface/GamesList.cpp",
	"../../interface/GamesList.h",
	"../../interface/gameplay.cpp",
	"../../interface/gameplay.h",
	"../../interface/n02.cpp",
	"../../interface/n02.h",
	"../../interface/n02module.h",
	"../../juce/juce.h",
	"../../juce/juce_Config.h",
	"../../modules/addEditIP.cpp",
	"../../modules/addEditIP.h",
	"../../modules/autorun.cpp",
	"../../modules/autorun.h",
	"../../modules/gameSelect.cpp",
	"../../modules/gameSelect.h",
	"../../modules/internalmodules.h",
	"../../modules/juceAbortableStatusWindow.cpp",
	"../../modules/juceAbortableStatusWindow.h",
	"../../modules/juceAddEditIPDialog.cpp",
	"../../modules/juceAddEditIPDialog.h",
	"../../modules/juceGameSelect.cpp",
	"../../modules/juceGameSelect.h",
	"../../modules/module.cpp",
	"../../modules/module.h",
	"../../modules/recorder.cpp",
	"../../modules/recorder.h",
	"../../modules/transport.cpp",
	"../../modules/transport.h",
	"../../modules/deferment/defermentInputManager.cpp",
	"../../modules/deferment/defermentInputManager.h",
	"../../modules/deferment/fixed.cpp",
	"../../modules/deferment/traditional.cpp",
	"../../modules/modDefStress/juceModDefStressTest.cpp",
	"../../modules/modDefStress/juceModDefStressTest.h",
	"../../modules/modDefStress/modDefStress.cpp",
	"../../modules/modDefStress/modDefStress.h",
	"../../modules/kaillera/juceKailleraMasterServersList.cpp",
	"../../modules/kaillera/juceKailleraMasterServersList.h",
	"../../modules/kaillera/juceKailleraServerConnection.cpp",
	"../../modules/kaillera/juceKailleraServerConnection.h",
	"../../modules/kaillera/juceKailleraServerGame.cpp",
	"../../modules/kaillera/juceKailleraServerGame.h",
	"../../modules/kaillera/juceKailleraServerSelect.cpp",
	"../../modules/kaillera/juceKailleraServerSelect.h",
	"../../modules/kaillera/kaillera_ClientCore.cpp",
	"../../modules/kaillera/kaillera_ClientCore.h",
	"../../modules/kaillera/kaillera_ClientMessaging.cpp",
	"../../modules/kaillera/kaillera_ClientMessaging.h",
	"../../modules/kaillera/kaillera_Instruction.h",
	"../../modules/kaillera/kaillera_uiServerWindow.cpp",
	"../../modules/kaillera/kaillera_uiServerWindowLists.cpp",
	"../../modules/kaillera/kaillera_uiServerWindowLists.h",
	"../../modules/kaillera/modKaillera.cpp",
	"../../modules/kaillera/modKaillera.h",
	"../../ui/clientgui.cpp",
	"../../ui/clientgui.h",
	"../../ui/juce02.h",
	"../../ui/juceLicenseComponent.cpp",
	"../../ui/juceLicenseComponent.h",
	"../../ui/license.cpp"
}
