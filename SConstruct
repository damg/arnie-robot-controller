env=Environment()
if ARGUMENTS.get("release", 0) == True:
    env.Append(CFLAGS=Split("-O2 -fomit-frame-pointer -ffast-math"))
else:
    env.Append(CFLAGS=Split("-g -ggdb -pg -Wall -Wextra"))

env.Append(CPPPATH=Split("#/include"))
env.Append(LIBPATH=Split("#/lib"))

Export("env")

subdirs=Split("src")

for e in subdirs:
    env.SConscript("%s/SConscript" % e)
