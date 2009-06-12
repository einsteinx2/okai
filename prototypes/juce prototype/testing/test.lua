
project.name = "n02test"

project.bindir = "../release"
project.libdir = "../release"

project.configs = { "Release" }

package = newpackage()
package.name = "n02test"

-- Pick which package you want. keep lib uncommented and dll commented for shared libarary
-- The other way around otherwise

package.kind = "exe"
package.language = "c++"
package.objdir = "../temp"

package.config["Release"].objdir = "../temp"
package.config["Release"].target = "test"
package.config["Release"].buildoptions  = { "-O2 -Wall" }
package.links = {"n02" }

package.includepaths = { 
    "../src/interface"
}

package.files = {
    "test.cpp",
    "../../interface/n02.h"
}

