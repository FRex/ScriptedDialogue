#pragma once
#include <vector>
#include <string>

struct lua_State;

enum ESCRIPTED_DIALOGUE_STATUS
{
    ESDS_ERROR = 0,
    ESDS_RETURN,
    ESDS_YIELD,
};

const int kScriptedDialogueNoAnswer = -9999;

class ScriptedDialogue
{
public:
    ScriptedDialogue();
    void setDialogueFunction(lua_State * L, int argscount = 0);
    ESCRIPTED_DIALOGUE_STATUS continueDialogue(int answer = kScriptedDialogueNoAnswer);
    const std::vector<std::string>& getAnswers() const;

private:
    void killThread();
    void popLinesFromThreadStack(lua_State * thread);

    lua_State * L = 0x0;
    int m_threadref;
    std::vector<std::string> m_lines;
    bool m_firstresume = true;

};
