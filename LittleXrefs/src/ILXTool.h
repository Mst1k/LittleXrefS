#pragma once

#include <vector>
#include <string>
#include <capstone\capstone.h>
#include "LittleXrefs.h"

using namespace LX;

struct FunctionReferenceList;
struct Function;
struct IReferenceEngine;

class ILXTool
{
private:
	csh	m_CapstoneDisasm;
	LittleXrefs* pLXRefs;
	std::vector<Function*> allFunctions;
	void ParseAllFunction();
protected:
	IReferenceEngine* RefsEngine;
public:
	ILXTool(LittleXrefs* _pLXRefs, cs_arch arch, cs_mode archMode);
	~ILXTool();

	void FindReferences(const std::string& className, uint64_t offset, FunctionReferenceList& ppOutReferenceList);
	LittleXrefs* getLittleXrefS();
	const std::vector<Function*>& getAllFunctions() const;
	void AddFunction(const std::string& name, const std::string& signature, uintptr_t offset);
	csh GetCapstoneHandle();
	void Run();
};

class LXARMTool : public ILXTool {
public:
	LXARMTool(LittleXrefs* pLXRefs);
	~LXARMTool();
};

class LXARM64Tool : public ILXTool {
public:
	LXARM64Tool(LittleXrefs* pLXRefs);
	~LXARM64Tool();
};


