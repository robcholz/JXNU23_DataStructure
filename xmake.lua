set_project("JXNU23_DataStructure")
set_languages("cxx17")


target("Lab1")
    set_kind("binary")
    add_files("src/Lab1.cpp")

target("Lab2")
    set_kind("binary")
    add_files("src/Lab2.cpp")

target("Lab3")
    set_kind("binary")
    add_files("src/Lab3.cpp")

target("Lab4")
    set_kind("binary")
    add_files("src/Lab4.cpp")

target("Lab5")
    set_kind("binary")
    add_files("src/Lab5.cpp")

target("Lab6")
    set_kind("binary")
    add_files("src/Lab6.cpp")

target("Lab7")
    set_kind("binary")
    add_files("src/lab7.rs")

target("Lab8")
    set_kind("binary")
    add_files("src/lab8.rs")

target("Lab9")
    set_kind("binary")
    add_files("src/Lab9.cpp")

target("Lab99(10)") -- name in this pattern for ordered run
    set_kind("binary")
    add_files("src/Lab10.cpp")
