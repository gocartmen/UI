import qbs

Project {
    minimumQbsVersion: "1.7.1"

    CppApplication {
        consoleApplication: true
        files: [
            "generatecppclass.cpp",
            "generatecppclass.h",
            "main.cpp",
            "nodeeditor.cpp",
            "nodeeditor.h",
            "textureselector.cpp",
            "textureselector.h",
            "textwriter.cpp",
            "textwriter.h",
            "types.h",
            "ui.cpp",
            "ui.h",
        ]

        cpp.cxxFlags: ["-fopenmp", "-ffast-math"]
        cpp.defines: base.concat((!qbs.debugInformation) ? ["QT_NO_DEBUG"] : ["QT_DEBUG"])


        cpp.includePaths: [product.sourceDirectory]

        //cpp.dynamicLibraries: ["glfw3", "glew32", "opengl32", "gdi32"]//windows
        //cpp.dynamicLibraries: ["GLEW", "GL", "glfw"]//ubuntu

        Properties {
            condition: qbs.targetOS.contains("windows") && qbs.architecture === "x86_64"
            cpp.cxxLanguageVersion: "c++20"

            cpp.dynamicLibraries: ["glew32", "opengl32", "glfw3", "gdi32"]
        }
        Properties {
            condition: qbs.targetOS.contains("linux") && qbs.architecture === "x86_64"
            cpp.cxxLanguageVersion: "c++17"

            cpp.dynamicLibraries: ["GLEW", "GL", "glfw", "stdc++fs"]
        }

        Group {
            name: "On Windows"
            condition: qbs.targetOS.contains("windows")
        }
        Group {
            name: "On Linux"
            condition: qbs.targetOS.contains("linux")
        }

        Group {     // Properties for the produced executable
            fileTagsFilter: "application"
            qbs.install: true
        }
    }
}
