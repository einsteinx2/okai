
project.name = "p2pssrv"

project.bindir = "../../../../../../release"
project.libdir = "../../../../../../release"

project.configs = { "Release" }

package = newpackage()
package.name = "p2pssrv"
package.kind = "exe"
package.language = "c++"
package.objdir = "../../../../../../temp"
package.linkoptions = { "-lpthread", "-lrt" }
-- package.linkoptions = { "-lws2_32", "-lwinmm" }

package.config["Release"].objdir = "../../../../../../temp/p2pssrv"
package.config["Release"].target = "p2pssrv"
package.config["Release"].defines       = { "N02_LINUX=1","USE_CPP_CUSTOM_INI=1" };
-- package.config["Release"].defines       = { "N02_WIN32=1","USE_CPP_CUSTOM_INI=1" };

package.includepaths = { 
    "../../../../../common",
    "../../.."
}

package.files = {
	"../../../../../common/BsdSocket.cpp",
	"../../../../../common/BsdSocket.h",
	"../../../../../common/ConfigurationManager.cpp",
	"../../../../../common/ConfigurationManager.h",
	"../../../../../common/DataQueue.h",
	"../../../../../common/DynamicAllocator.h",
	"../../../../../common/DynamicArray.h",
	"../../../../../common/DynamicBuffer.h",
	"../../../../../common/DynamicOrderedArray.h",
	"../../../../../common/DynamicQueue.h",
	"../../../../../common/DynamicStack.h",
	"../../../../../common/GlobalTimer.h",
	"../../../../../common/LinkedList.h",
	"../../../../../common/ListGenerics.h",
	"../../../../../common/Logger.cpp",
	"../../../../../common/Logger.h",
	"../../../../../common/PosixThread.cpp",
	"../../../../../common/PosixThread.h",
	"../../../../../common/SocketAddress.cpp",
	"../../../../../common/SocketAddress.h",
	"../../../../../common/StaticAllocator.h",
	"../../../../../common/StaticArray.h",
	"../../../../../common/StaticBuffer.h",
	"../../../../../common/StaticOrderedArray.h",
	"../../../../../common/StaticQueue.h",
	"../../../../../common/StaticStack.h",
	"../../../../../common/StringUtils.cpp",
	"../../../../../common/StringUtils.h",
	"../../../../../common/UdpSocket.h",
	"../../../../../common/_common.cpp",
	"../../../../../common/_common.h",
	"../../../../../common/common.cpp",
	"../../../../../common/common.h",
	"../../../../../common/trace.cpp",
	"../../../../../common/trace.h",
--	"../../../../../common/SEH_win32.h",
--	"../../../../../common/SEH_win32.cpp",
	"../../../../../common/signals_GNU.h",
	"../../../../../common/signals_GNU.cpp",
	"../../ssrv.cpp"
}

