#include <hook.h>
#include <mod.h>
#include <logger.h>
#include <symbol.h>
#include <static_symbol.h>
#include <Actor.hpp>
#include <Mob.hpp>
#include <string>
#include <stl/vector>
#include <stl/unordered_map>
#include <stl/utility>
#include <memory>
#include <commontypes.hpp>
#include <jni.h>
#include <ActorUniqueID.hpp>

class MolangVariableIndex
{
	char fill[4];
};

class MolangScriptArg
{
	char fill[4];	// type ?
	char fill2[4];	//
	char value[4];	// float value
	char fill3[4];	//
	char fill4[64]; // union
	char type;
};

class MolangVariable
{
public:
	char fill[24];	// hashedString name ?
	short varIndex; //

public:
	static HashedString *getVariableName(MolangVariableIndex);
	static MolangVariableIndex getVariableIndex(const HashedString &);
};

class MolangVariableMap
{
	char fill[12];
	std::vector<std::unique_ptr<MolangVariable>> v;
};

typedef MolangVariableMap *(*GetVariableMapPtr)(Actor *);
typedef int (*SetVariablePtr)(MolangVariableMap *, MolangVariableIndex, MolangScriptArg *);
static GetVariableMapPtr getVariableMapFunc = nullptr;
static SetVariablePtr setVariableFunc = nullptr;

void SetMolangVarible(MolangVariableMap *varMap, MolangVariableIndex index, float value)
{
	int *val = new int[27]{0};
	char *tmp = (char *)val;
	for (int i = 0; i < 81; i++)
		tmp[i] = 0;
	val[2] = *((int *)&value);
	setVariableFunc(varMap, index, (MolangScriptArg *)val);
	delete[] val;
}

class MainModule : public Module
{
public:
	MainModule(const char *id) : Module(id){};
	static stl::unordered_map<long long, stl::vector<stl::pair<MolangVariableIndex, float>>> variableMap;
	virtual void initialize()
	{
		DLHandleManager::initializeHandle("libminecraftpe.so", "mcpe");
		getVariableMapFunc = (GetVariableMapPtr)SYMBOL("mcpe", "_ZN5Actor18getMolangVariablesEv");
		setVariableFunc = (SetVariablePtr)SYMBOL("mcpe", "_ZN17MolangVariableMap17setMolangVariableE19MolangVariableIndexRK15MolangScriptArg");
		/*
		HookManager::addCallback(
			SYMBOL("mcpe", "_ZN17MolangVariableMap17setMolangVariableERK12HashedStringRK15MolangScriptArg"),
			LAMBDA((MolangVariableMap *, const HashedString &str, const MolangScriptArg &arg), {
				Logger::debug("test", str.c_str());
			}, ),
			HookManager::RETURN | HookManager::LISTENER);
		HookManager::addCallback(
			SYMBOL("mcpe", "_ZN17MolangVariableMap17setMolangVariableEyPKcRK15MolangScriptArgb"),
			LAMBDA((MolangVariableMap *, unsigned long long, const char *str, const MolangScriptArg &arg, bool), {
				Logger::debug("test", str);
			}, ),
			HookManager::RETURN | HookManager::LISTENER);
		HookManager::addCallback(
			SYMBOL("mcpe", "_ZN17MolangVariableMap17setMolangVariableEyPKcRK15MolangScriptArg"),
			LAMBDA((MolangVariableMap *, unsigned long long, const char *str, const MolangScriptArg &arg), {
				Logger::debug("test", str);
			}, ),
			HookManager::RETURN | HookManager::LISTENER);
		*/
		/*
		HookManager::addCallback(
			SYMBOL("mcpe", "_ZN17MolangVariableMap17setMolangVariableE19MolangVariableIndexRK15MolangScriptArg"),
			LAMBDA((MolangVariableMap * ptr, MolangVariableIndex idx, void *arg), {
				Logger::debug("test", "%s=%f",
							  MolangVariable::getVariableName(idx)->c_str(),
							  *(float *)(arg + 8));
			}, ),
			HookManager::RETURN | HookManager::LISTENER);
		*/
		HookManager::addCallback(
			SYMBOL("mcpe", "_ZN3Mob35updateEntitySpecificMolangVariablesER12RenderParams"),
			LAMBDA((Mob * ptr, void *renderParams), {
				Actor *actor = (Actor *)ptr;
				ActorUniqueID *uid = actor->getUniqueID();
				if (MainModule::variableMap.count(uid->id) > 0)
				{
					for (auto &i : MainModule::variableMap[uid->id])
						SetMolangVarible(getVariableMapFunc(actor), i.first, i.second);
					MainModule::variableMap[uid->id].clear();
				}
				// Logger::debug("test2", "%p,%p", ptr, getVariableMapFunc((Actor *)ptr));
			}, ),
			HookManager::RETURN | HookManager::LISTENER);
	}
};
stl::unordered_map<long long, stl::vector<stl::pair<MolangVariableIndex, float>>> MainModule::variableMap;

MAIN
{
	Module *main_module = new MainModule("AnimationComponent");
}

extern "C"
{
#define __EXPORT__(RET, NAME, ...) JNIEXPORT RET JNICALL Java_cutehusky_AnimationComponent_MolangHelper_native##NAME(JNIEnv *env, jclass clazz, ##__VA_ARGS__)

	__EXPORT__(void, Initial, jlong uid)
	{
		MainModule::variableMap.insert(stl::make_pair(uid, stl::vector<stl::pair<MolangVariableIndex, float>>()));
	}

	__EXPORT__(void, Release, jlong uid)
	{
		MainModule::variableMap.erase(uid);
	}

	__EXPORT__(jint, GetVariableIndex, jstring varName)
	{
		const char *cVarName = env->GetStringUTFChars(varName, 0);
		HashedString hVarName(cVarName);
		env->ReleaseStringUTFChars(varName, cVarName);
		auto index = MolangVariable::getVariableIndex(hVarName);
		return (jint)(*((int *)&index));
	}

	__EXPORT__(void, SetVariableByIndex, jlong uid, jint varIndex, jfloat value)
	{
		MolangVariableIndex *index = (MolangVariableIndex *)&varIndex;
		MainModule::variableMap[uid].push_back(stl::make_pair(*index, value));
	}

	__EXPORT__(void, SetVariableByName, jlong uid, jstring varName, jfloat value)
	{
		const char *cVarName = env->GetStringUTFChars(varName, 0);
		HashedString hVarName(cVarName);
		env->ReleaseStringUTFChars(varName, cVarName);
		MainModule::variableMap[uid].push_back(stl::make_pair(MolangVariable::getVariableIndex(hVarName), value));
	}
}