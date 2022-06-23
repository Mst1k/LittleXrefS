#include "ILXTool.h"
#include "Function.h"
#include "ReferenceEngine.h"
#include "FunctionReferenceList.h"

void ILXARMTool::ParseAllFunction()
{
    auto& allFuncJson = getLittleXrefS()->getDumpJsonObj()["ScriptMethod"];

    for (size_t i = 0; i < allFuncJson.size(); i++)
    {
        const auto& currFunc = allFuncJson[i];

        AddFunction(currFunc["Name"].asString(), currFunc["Signature"].asString(), currFunc["Address"].asUInt());
    }
}

ILXARMTool::ILXARMTool(LittleXrefs* pLXRefs)
    : ILXTool(pLXRefs, CS_ARCH_ARM, CS_MODE_ARM)
{
    RefsEngine = new ArmReferenceEngine(GetCapstoneHandle());
    ParseAllFunction();
}

ILXARMTool::~ILXARMTool()
{
}

ILXTool::ILXTool(LittleXrefs* _pLXRefs, cs_arch arch, cs_mode archMode)
    : pLXRefs(_pLXRefs)
{
    //opening the handle of the capstone disasm
    if (cs_open(arch, archMode, &m_CapstoneDisasm) != CS_ERR_OK)
        throw "Failed Opening Handle of Disassembler";

    // setting detailed mode on
    if (cs_option(m_CapstoneDisasm, cs_opt_type::CS_OPT_DETAIL, true) != CS_ERR_OK)
    {
        cs_close(&m_CapstoneDisasm);

        throw "Failed enabling Detailed Options";
    }
}

ILXTool::~ILXTool()
{
}

void ILXTool::FindReferences(const std::string& className, uint64_t offset, FunctionReferenceList& ppOutReferenceList)
{
    std::vector<Function*> candidatesFunctions;

    for (auto* function : getAllFunctions())
    {
        if (function->ContainsParamType(className))
            candidatesFunctions.push_back(function);
    }

    if (candidatesFunctions.size() > 0)
    {
        for (auto* candidateFunc : candidatesFunctions)
            RefsEngine->FindRefereces(candidateFunc, className, offset, ppOutReferenceList);
    }
}

LittleXrefs* ILXTool::getLittleXrefS()
{
    return pLXRefs;
}

const std::vector<Function*>& ILXTool::getAllFunctions() const
{
    return allFunctions;
}

void ILXTool::AddFunction(const std::string& name, const std::string& signature, uintptr_t offset)
{
    allFunctions.push_back(new Function(name, signature, getLittleXrefS()->getAssemblyEntry() + offset, offset));
}

csh ILXTool::GetCapstoneHandle()
{
    return m_CapstoneDisasm;
}