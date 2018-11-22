// SpringScriptEngine.cpp : Defines the exported functions for the DLL application.
//
#include "SpringScriptEngine.h"
#include "CScriptEngine.h"
#include "CVariableMan.h"

#define HANDLECHECK(p) if (p == NULL) { return FALSE; }
#define SS(p) ((CScriptEngine*)p)
#define VM(p) ((CVariableMan*)p)
using namespace std;

DLLEXPORT HSPRINGSCRIPT SpringScript_CreateInstance() {
	//インスタンスを作成
	CScriptEngine* pIns = new(nothrow) CScriptEngine;
	return (HSPRINGSCRIPT)pIns;
}

DLLEXPORT BOOL SpringScript_DestroyInstance(HSPRINGSCRIPT hSpringScript) {
	HANDLECHECK(hSpringScript);

	//インスタンスを破棄
	delete hSpringScript;
	return TRUE;
}

DLLEXPORT BOOL SpringScript_SetErrorCallbk(HSPRINGSCRIPT hSpringScript, HWND hWnd, ErrorCallbk pFunc, void *pUserData) {
	HANDLECHECK(hSpringScript);

	SS(hSpringScript)->m_em.SetOwnerWnd(hWnd);
	SS(hSpringScript)->m_em.SetCallBk(pFunc, pUserData);
	return TRUE;
}

DLLEXPORT BOOL SpringScript_Load(HSPRINGSCRIPT hSpringScript, const char *pszFileName) {
	HANDLECHECK(hSpringScript);

	if (!SS(hSpringScript)->Load(pszFileName))
		return FALSE;
	return TRUE;
}

DLLEXPORT BOOL SpringScript_Run(HSPRINGSCRIPT hSpringScript) {
	HANDLECHECK(hSpringScript);

	SS(hSpringScript)->Run();
	return TRUE;
}

DLLEXPORT BOOL SpringScript_Break(HSPRINGSCRIPT hSpringScript) {
	HANDLECHECK(hSpringScript);

	SS(hSpringScript)->Break();
	return TRUE;
}

DLLEXPORT BOOL SpringScript_Exit(HSPRINGSCRIPT hSpringScript) {
	HANDLECHECK(hSpringScript);

	SS(hSpringScript)->Exit();
	return TRUE;
}

DLLEXPORT BOOL SpringScript_RegisterCommand(HSPRINGSCRIPT hSpringScript, const char *pszName, const char *pszPrm, seCOMMAND pFunc, void *pUserData) {
	HANDLECHECK(hSpringScript);

	if (!SS(hSpringScript)->RegisterCommand(string(pszName), string(pszPrm), pFunc, pUserData))
		return FALSE;
	return TRUE;
}

DLLEXPORT BOOL SpringScript_UnregisterCommand(HSPRINGSCRIPT hSpringScript, const char *pszName) {
	HANDLECHECK(hSpringScript);

	if (!SS(hSpringScript)->UnregisterCommand(string(pszName)))
		return FALSE;
	return TRUE;
}

DLLEXPORT BOOL SpringScript_GoTo(HSPRINGSCRIPT hSpringScript, const char *pszLabel) {
	HANDLECHECK(hSpringScript);

	if (!SS(hSpringScript)->GoTo(string(pszLabel), false))
		return FALSE;
	return TRUE;
}

DLLEXPORT BOOL SpringScript_GetPostion(HSPRINGSCRIPT hSpringScript, PostionInfo *pi) {
	HANDLECHECK(hSpringScript);

	*pi = SS(hSpringScript)->GetPostion();
	return TRUE;
}

DLLEXPORT BOOL SpringScript_GetPostionLH(HSPRINGSCRIPT hSpringScript, PostionInfo *pi) {
	HANDLECHECK(hSpringScript);

	*pi = SS(hSpringScript)->GetPostionLH();
	return TRUE;
}

DLLEXPORT BOOL SpringScript_SetPostion(HSPRINGSCRIPT hSpringScript, const PostionInfo *pi) {
	HANDLECHECK(hSpringScript);

	SS(hSpringScript)->SetPostion(*pi);
	return TRUE;
}

DLLEXPORT BOOL SpringScript_SetPostionForce(HSPRINGSCRIPT hSpringScript, const PostionInfo *pi) {
	HANDLECHECK(hSpringScript);

	SS(hSpringScript)->SetPostionForce(*pi);
	return TRUE;
}

DLLEXPORT BOOL SpringScript_GetLineNum(HSPRINGSCRIPT hSpringScript, size_t *pVal) {
	HANDLECHECK(hSpringScript);
	if (pVal == NULL)
		return FALSE;
	*pVal = SS(hSpringScript)->GetLine();
	return TRUE;
}

DLLEXPORT BOOL SpringScript_Eval(HSPRINGSCRIPT hSpringScript, const char *pszExpression, int *out) {
	HANDLECHECK(hSpringScript);
	if (out == NULL)
		return FALSE;

	*out = SS(hSpringScript)->Eval_i(string(pszExpression));
	return TRUE;
}

DLLEXPORT BOOL SpringScript_Evalf(HSPRINGSCRIPT hSpringScript, const char *pszExpression, float *out) {
	HANDLECHECK(hSpringScript);
	if (out == NULL)
		return FALSE;

	*out = SS(hSpringScript)->Eval_f(string(pszExpression));
	return TRUE;
}

DLLEXPORT UINT SpringScript_GetCmdArgCount(const CommandArg *arg) {
	if (arg == NULL)
		return 0;

	return arg->ArgList.size();
}

DLLEXPORT CVariableMan::VarType SpringScript_GetCmdArgType(CommandArg *arg, int index) {
	if (arg == NULL)
		return CVariableMan::VARTYPE_UNKNOWN;
	if (arg->ArgList.size() <= index || index < 0)
		return CVariableMan::VARTYPE_UNKNOWN;

	return arg->ArgList[index].GetType();
}

DLLEXPORT HSPRINGSCRIPTVAR SpringScript_GetCmdArgPtr(CommandArg *arg, int index) {
	if (arg == NULL)
		return NULL;
	if (arg->ArgList.size() <= index || index < 0)
		return NULL;

	return (HSPRINGSCRIPTVAR)&arg->ArgList[index];
}

DLLEXPORT INT SpringScript_GetVarValInt(HSPRINGSCRIPTVAR *var) {
	if (var == NULL)
		return 0;
	if (VM(var)->GetType() != CVariableMan::VARTYPE_INTEGER)
		return 0;
	return *((INT*)VM(var)->GetData());
}

DLLEXPORT BOOL SpringScript_GetVarValStr(HSPRINGSCRIPTVAR *var, char *lpString, int nMaxCount) {
	if (var == NULL)
		return FALSE;
	if (VM(var)->GetType() != CVariableMan::VARTYPE_STRING)
		return FALSE;

	strncpy_s(lpString, nMaxCount, ((string*)VM(var)->GetData())->c_str(), _TRUNCATE);
	return TRUE;
}

DLLEXPORT INT SpringScript_GetVarValStrLen(HSPRINGSCRIPTVAR *var) {
	if (var == NULL)
		return 0;
	if (VM(var)->GetType() != CVariableMan::VARTYPE_STRING)
		return 0;

	return ((string*)VM(var)->GetData())->length();
}

DLLEXPORT FLOAT SpringScript_GetVarValFloat(HSPRINGSCRIPTVAR *var) {
	if (var == NULL)
		return 0;
	if (VM(var)->GetType() != CVariableMan::VARTYPE_FLOAT)
		return 0;
	return *((FLOAT*)VM(var)->GetData());
}

DLLEXPORT INT SpringScript_GetVarValHnd(HSPRINGSCRIPTVAR *var) {
	if (var == NULL)
		return 0;
	if (VM(var)->GetType() != CVariableMan::VARTYPE_HANDLE)
		return 0;
	return *((INT*)VM(var)->GetData());
}

DLLEXPORT HSPRINGSCRIPTVAR SpringScript_GetVarValRef(HSPRINGSCRIPTVAR *var) {
	if (var == NULL)
		return NULL;
	if (VM(var)->GetType() != CVariableMan::VARTYPE_VARIABLE)
		return NULL;
	return (HSPRINGSCRIPTVAR)(CVariableMan*)VM(var)->GetData();
}

DLLEXPORT BOOL SpringScript_SetVarVal(HSPRINGSCRIPTVAR *var, const void *val, CVariableMan::VarType type) {
	if (var == NULL)
		return FALSE;
	if (val == NULL)
		return FALSE;
	switch (type)
	{
	case CVariableMan::VARTYPE_INTEGER:
		VM(var)->Set(*(int*)val);
		break;
	case CVariableMan::VARTYPE_STRING:
		VM(var)->Set(&string((const char*)val));
		break;
	case CVariableMan::VARTYPE_FLOAT:
		VM(var)->Set(*(float*)val);
		break;
	case CVariableMan::VARTYPE_HANDLE:
		VM(var)->SetAsHandle(*(int*)val);
		break;
	case CVariableMan::VARTYPE_VARIABLE:
		VM(var)->Set((CVariableMan*)val);
		break;
	default:
		return FALSE;

	}
	return TRUE;
}