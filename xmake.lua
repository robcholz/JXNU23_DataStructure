set_project("JXNU23_DataStructure")
set_languages("cxx17")


--add_requires("cargo::serde 1.0.193",{ configs = { features = {"derive"}},alias="serde"})
--add_requires("cargo::serde_json 1.0.109",{alias="serde_json"})


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

--target("Lab8")
--    set_kind("binary")
--  add_files("src/lab8.rs")
--  add_packages("serde")
--  add_packages("serde_json")

target("Lab9")
    set_kind("binary")
    add_files("src/Lab9.cpp")

target("Lab10")
    set_kind("binary")
    add_files("src/Lab10.cpp")
