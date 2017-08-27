#include <iostream>
#include "ScriptedDialogue.hpp"
#include <lua.hpp>

int main(int argc, char ** argv)
{
    lua_State * L = luaL_newstate();
    if(!L)
    {
        std::printf("luaL_newstate failed\n");
        return 1;
    }
    luaL_openlibs(L);
    if(luaL_dofile(L, "main.lua"))
    {
        std::printf("%s\n", lua_tostring(L, -1));
        lua_pop(L, 1);
        lua_close(L);
        return 2;
    }

    ScriptedDialogue dia;
    lua_getglobal(L, "test");
    dia.setDialogueFunction(L);

    int nextanswer = kScriptedDialogueNoAnswer;
    while(true)
    {
        auto status = dia.continueDialogue(nextanswer);

        for(const auto& opt : dia.getAnswers())
            std::cout << opt << std::endl;

        if(dia.getAnswers().size() > 1)
        {
            std::cin >> nextanswer;
        }
        else
        {
            nextanswer = kScriptedDialogueNoAnswer;
            std::string str;
            std::getline(std::cin, str);
        }
        if(status != ESDS_YIELD)
            break;
    }//while true
    lua_close(L);
    return 0;
}
