// main.cpp -- main function for testing config parsing

#define _CRT_SECURE_NO_WARNINGS // "secure" CRT not available on all platforms  --add this at the top of all CPP files that give "function or variable may be unsafe" warnings

#include "../CNTK/ConfigEvaluator.h"

using namespace Microsoft::MSR::CNTK::Config;

#ifndef let
#define let const auto
#endif

namespace Microsoft { namespace MSR { namespace CNTK { namespace Config {
    shared_ptr<Object> MakeExperimentalComputationNetwork(const ConfigRecord &) { return nullptr; }
    shared_ptr<Object> MakeExperimentalComputationNode(const ConfigRecord &) { return nullptr; }
}}}}

#if 0
// notes on integrating
if (config.Exists("NDLNetworkBuilder"))
{
    ConfigParameters configNDL(config("NDLNetworkBuilder"));
    netBuilder = (IComputationNetBuilder<ElemType>*)new NDLBuilder<ElemType>(configNDL);
}
else if (config.Exists("ExperimentalNetworkBuilder"))
{
    ConfigParameters sourceCode(config("ExperimentalNetworkBuilder"));
    // get sourceCode as a nested string that contains the inside of a dictionary (or a dictionary)
    netBuilder = (IComputationNetBuilder<ElemType>*)new ExperimentalNetworkBuilder<ElemType>(sourceCode);
}
// netBuilder is a wrapper with these methods to create a ComputationNetwork:; see NDLNetworkBuilder.h
ComputationNetwork<ElemType>* net = startEpoch < 0 ? netBuilder->BuildNetworkFromDescription() :
    netBuilder->LoadNetworkFromFile(modelFileName);
// LoadNetworkFromFile() -> NDLNetworkBuilder.h LoadFromConfig() 
// -> NDLUtil.h NDLUtil::ProcessNDLScript()
// does multiple passes calling ProcessPassNDLScript()
// -> NetworkDescriptionLanguage.h NDLScript::Evaluate
// which sometimes calls into NDLNodeEvaluator::Evaluate()
// NDLNodeEvaluator: implemented by execution engines to convert script to approriate internal formats
// here: SynchronousNodeEvaluator in SynchronousExecutionEngine.h
// SynchronousNodeEvaluator::Evaluate()   --finally where the meat is
//  - gets parameters from config and translates them into ComputationNode
//    i.e. corrresponds to our MakeRuntimeObject<ComputationNode>()
//  - creates all sorts of ComputationNode types, based on NDLNode::GetName()
//     - parses parameters depending on node type   --this is the NDL-ComputationNode bridge
//     - creates ComputationNodes with an additional layer of wrappers e.g. CreateInputNode()
//     - then does all sorts of initialization depending on mode type
//  - can initialize LearnableParameters, incl. loading from file. WHY IS THIS HERE?? and not in the node??
//  - for standard nodes just creates them by name (like our classId) through m_net.CreateComputationNode()
// tags:
//  - tags are not known to ComputationNode, but to Network
//  - processed by SynchronousNodeEvaluator::ProcessOptionalParameters() to sort nodes into special node-group lists such as m_featureNodes (through SetOutputNode())

// notes:
//  - InputValue nodes are created from 4 different names: InputValue, SparseInputvalue, ImageInput, and SparseImageInput
//  - for SparseInputvalue, it checks against InputValue::SparseTypeName(), while using a hard-coded string for ImageInput and SparseImageInput
//  - there is also SparseLearnableParameter, but that's a different ComputationNode class type
#endif

// OUTDATED--moved to CNTK project

wstring standardFunctions =
L"Print(value, format='') = new PrintAction [ what = value /*; how = format*/ ] \n"
L"Fail(msg) = new FailAction [ what = msg ] \n"
L"RequiredParameter(message) = Fail('RequiredParameter: ' + message) \n"
L"Format(value, format) = new StringFunction [ what = 'Format' ; arg = value ; how = format ] \n"
L"Replace(s, from, to) = new StringFunction [ what = 'Replace' ; arg = s ; replacewhat = from ; withwhat = to ] \n"
L"Substr(s, begin, num) = new StringFunction [ what = 'Substr' ; arg = s ; pos = begin ; chars = num ] \n"
L"Chr(c) = new StringFunction [ what = 'Chr' ;  arg = c ] \n"
L"Floor(x)  = new NumericFunction [ what = 'Floor' ;  arg = x ] \n"
L"Length(x) = new NumericFunction [ what = 'Length' ; arg = x ] \n"
L"Ceil(x) = -Floor(-x) \n"
L"Round(x) = Floor(x+0.5) \n"
L"Abs(x) = if x >= 0 then x else -x \n"
L"Sign(x) = if x > 0 then 1 else if x < 0 then -1 else 0 \n"
L"Min(a,b) = if a < b then a else b \n"
L"Max(a,b) = if a > b then a else b \n"
L"Fac(n) = if n > 1 then Fac(n-1) * n else 1 \n"
L""
L""
L""
L""
L""
L""
;

wstring computationNodes =      // BUGBUG: optional args not working yet, some scope problem causing a circular reference
L"Mean(z, tag='') = new ComputationNode [ class = 'MeanNode' ; inputs = z ; optionalTag = 'tag' ]\n"
L"InvStdDev(z, tag='') = new ComputationNode [ class = 'InvStdDevNode' ; inputs = z ; optionalTag = 'tag' ]\n"
L"PerDimMeanVarNormalization(feat,mean,invStdDev, tag='') = new ComputationNode [ class = 'PerDimMeanVarNormalizationNode' ; inputs = feat:mean:invStdDev ; optionalTag = 'tag' ]\n"
L"Parameter(outD, inD/*, tag=''*/) = new ComputationNode [ class = 'LearnableParameterNode' ; outDim = outD ; inDim = inD /*; optionalTag = 'tag'*/ ]\n"
L"Input(dim) = Parameter(dim,1/*,tag='features'*/)   // TODO: for now \n"
L"RowSlice(firstRow, rows, features, tag='') = new ComputationNode [ class = 'RowSliceNode' ; inputs = features ; first = firstRow ; num = rows ; optionalTag = 'tag' ]\n"
L"Delay(in, delay, tag='') = new ComputationNode [ class = 'DelayNode' ; input = in ; deltaT = -delay ; optionalTag = 'tag' ]\n"
L"Sigmoid(z, tag='') = new ComputationNode [ class = 'SigmoidNode' ; inputs = z ; optionalTag = 'tag' ]\n"
L"Log(z, tag='') = new ComputationNode [ class = 'LogNode' ; inputs = z ; optionalTag = 'tag' ]\n"
L"CrossEntropyWithSoftmax(labels, outZ, tag='') = new ComputationNode [ class = 'CrossEntropyWithSoftmaxNode' ; inputs = labels:outZ ; optionalTag = 'tag' ]\n"
L"ErrorPrediction(labels, outZ, tag='') = new ComputationNode [ class = 'ErrorPredictionNode' ; inputs = labels:outZ ; optionalTag = 'tag' ]\n"
L" \n"
L" \n"
L" \n"
L" \n"
L" \n"
L" \n"
L" \n"
;

wstring commonMacros =  // TODO: rename rows and cols to inDim and outDim or vice versa, whichever it is
L"BFF(in, rows, cols) = [ B = Parameter(rows, 1/*init = fixedvalue, value = 0*/) ; W = Parameter(rows, cols) ; z = W*in+B ] \n"
L"SBFF(in, rows, cols) = [ Eh = Sigmoid(BFF(in, rows, cols).z) ] \n "
L"MeanVarNorm(feat) = PerDimMeanVarNormalization(feat, Mean(feat), InvStdDev(feat)) \n"
L"LogPrior(labels) = Log(Mean(labels)) \n"
L""
L""
L""
L""
;



int wmain(int /*argc*/, wchar_t* /*argv*/[])
{
    try
    {
        // collecting all sorts of test cases here
        wchar_t * parserTests[] = 
        {
            L"do = Parameter(13,42) * Input(42) + Parameter(13,1)"
            ,
            L"do = Print(array [1..10] (i=>i*i))"
            ,
            L"do = new PrintAction [ what = 'abc' ]"
            ,
            L"do = Print(new StringFunction [ x = 13 ; y = 42 ; what = 'Format' ; how = '.2' ; arg = x*y ])"
            ,
            L"do = Print(\"new StringFunction [ what = 'Format' ; how = '.2' ; arg = '13 > 42' ]\")"
            ,
            L"do = new PrintAction [ what = if 13 > 42 || 12 > 1 then 'Hello World' + \"!\" else 'Oops?']"
            ,
            L"i2s(i) = new StringFunction [ what = 'Format' ; arg = i ; how = '.2' ] ; do = Print('result=' + i2s((( [ v = (i => i + delta) ].v(5)))+13)) ; delta = 42 "
            ,
            L"do = Print(1+2*3) : Print('hello'+' world')"
            ,
            L"do = Print(Format( (13:(fortytwo:1):100), '')) ; fortytwo=42 "
            ,
            L"do = Print(val) ; val=if !false then 42 else -+-++-13:[a='a';b=42]:+14; arr = array [1..10] (i => 2*i)"
            ,
            L"do = Print(arg) ; N = 5 ; arr = array [1..N] (i => if i < N then arr[i+1]*i else N) ; arg = arr "
            ,
            L"do = Print(val) ; val = [ v = (i => i + offset) ].v(42) ; offset = 13 "
            ,
            L" \n"
            L"do = Print(val) \n"
            L"val = new NDLComputationNetwork [\n"
            L"  featDim=40*31 ; labelDim=9000 ; hiddenDim=2048 ; numHiddenLayers = 3 \n"
            L"  myFeatures = Input(featDim) ; myLabels = Input(labelDim) \n"
            L"  featNorm = MeanVarNorm(myFeatures) \n"
            L"  HiddenStack(layer) = if layer > 1 then SBFF(HiddenStack(layer - 1).Eh, hiddenDim, hiddenDim) else SBFF(featNorm, hiddenDim, featDim) \n"
            L"  outLayer = BFF(HiddenStack(numHiddenLayers).Eh, labelDim, hiddenDim) \n"
            L"  outZ = outLayer.z \n"
            L"  CE = CrossEntropyWithSoftmax(myLabels, outZ) \n"
            L"  Err = ErrorPrediction(myLabels, outZ) \n"
            L"  logPrior = LogPrior(myLabels) \n"
            L"  ScaledLogLikelihood = outZ - logPrior \n"
            L"]\n"
            ,
            L"do = Print(fac(5)) ; val = RequiredParameter('need to specify val') ; fac(i) = if i > 1 then fac(i-1)*i else i "
            ,
            L"do = new PrintAction [ what = val ] ; fib(n) = [ vals = array[1..n] (i => if i < 3 then i-1 else vals[i-1]+vals[i-2]) ].vals ; val = fib(10) "
            ,
            L" \n"
            L"do = Print(val) \n"
            L"val = new NDLComputationNetwork [\n"
            L"  featDim=40*31 ; labelDim=9000 ; hiddenDim=2048 ; numHiddenLayers = 3 \n"
            L"  myFeatures = Input(featDim) ; myLabels = Input(labelDim) \n"
            L"  featNorm = MeanVarNorm(myFeatures) \n"
            //L"  layers/*[layer=1..numHiddenLayers]*/ = array[1..numHiddenLayers] (layer => if layer > 1 then SBFF(layers[layer-1].Eh, hiddenDim, hiddenDim) else SBFF(featNorm, hiddenDim, featDim)) \n"
            L"  layers[layer:1..numHiddenLayers] = if layer > 1 then SBFF(layers[layer-1].Eh, hiddenDim, hiddenDim) else SBFF(featNorm, hiddenDim, featDim) \n"
            L"  outLayer = BFF(layers[numHiddenLayers].Eh, labelDim, hiddenDim) \n"
            L"  outZ = outLayer.z //+ Delay(outZ, 1) \n"
            L"  CE = CrossEntropyWithSoftmax(myLabels, outZ) \n"
            L"  Err = ErrorPrediction(myLabels, outZ) \n"
            L"  logPrior = LogPrior(myLabels) \n"
            L"  ScaledLogLikelihood = outZ - logPrior \n"
            L"]\n"
            ,
            L" \n"   // this fails because dict is outside val; expression name is not local to it
            L"do = Print(val) \n"
            L"dict = [ outY = Input(13) ] ; val = new NDLComputationNetwork [ outZ = dict.outY \n"
            L"]\n"
            ,
            NULL
        };
        let first = 0;// 12;
        bool oneOnly = first > 0;
        for (size_t i = first; parserTests[i]; i++)
        {
            fprintf(stderr, "\n### Test %d ###\n\n", i), fflush(stderr);
            let parserTest = parserTests[i];
            let expr = ParseConfigString(standardFunctions + computationNodes + commonMacros + parserTest);
            //expr->Dump();
            Do(expr);
            if (oneOnly)
                break;
        }
        //ParseConfigFile(L"c:/me/test.txt")->Dump();
        return EXIT_SUCCESS;
    }
    catch (const ConfigError & err)
    {
        err.PrintError();
        return EXIT_FAILURE;
    }
}
