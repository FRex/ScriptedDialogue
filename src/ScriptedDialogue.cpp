#include "ScriptedDialogue.hpp"
#include <lua.hpp>
#include <cstdio>

ScriptedDialogue::ScriptedDialogue() : m_threadref(LUA_NOREF)
{
}

void ScriptedDialogue::setDialogueFunction(lua_State * L, int argscount)
{
    this->L = L;
    killThread();
    auto thread = lua_newthread(L);
    m_threadref = luaL_ref(L, LUA_REGISTRYINDEX);
    lua_xmove(L, thread, 1 + argscount);
}

ESCRIPTED_DIALOGUE_STATUS ScriptedDialogue::continueDialogue(int answer)
{
    lua_rawgeti(L, LUA_REGISTRYINDEX, m_threadref);
    auto thread = lua_tothread(L, -1);
    lua_pop(L, 1);
    if(answer != kScriptedDialogueNoAnswer)
        lua_pushinteger(thread, answer);

    const int ret = lua_resume(thread, lua_gettop(thread) - m_firstresume);
    m_firstresume = false;
    if(ret == 0)
    {
        popLinesFromThreadStack(thread);
        return ESDS_RETURN;
    }
    else if(ret == LUA_YIELD)
    {
        popLinesFromThreadStack(thread);
        return ESDS_YIELD;
    }
    else
    {
        m_lines.clear();
        std::fprintf(stderr, "%s\n", lua_tostring(thread, -1));
        lua_pop(thread, 1);
        return ESDS_ERROR;
    }
}

const std::vector<std::string>& ScriptedDialogue::getAnswers() const
{
    return m_lines;
}

void ScriptedDialogue::killThread()
{
    luaL_unref(L, LUA_REGISTRYINDEX, m_threadref);
    m_threadref = LUA_NOREF;
    m_firstresume = true;
}

void ScriptedDialogue::popLinesFromThreadStack(lua_State * thread)
{
    m_lines.clear();
    for(int i = 1; i <= lua_gettop(thread); ++i)
        m_lines.push_back(lua_tostring(thread, i));

    lua_settop(thread, 0);
}
