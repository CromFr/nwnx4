#include "nwnx_include"
#include "nwnx_sql"
#include "nwnx_funcs"
#include "nwnx_time"

object oModule = GetModule();
location lStart = GetStartingLocation();
object oArea = GetAreaFromLocation(lStart);
object oContainer = CreateObject(OBJECT_TYPE_PLACEABLE, "plc_mc_lbox01", lStart);

void ShutdownIfFailed(){
	if(GetLocalInt(GetModule(), "shutting_down")){
		WriteTimestampedLogEntry("!! Shutting down server because of failed asserts");
		NWNXSetString("SRVADMIN", "SHUTDOWNNWN2SERVER", "", 0, "");
	}
}

void Assert(int bCond, string sFunction, int nLine, string sMessage=""){
	if(!bCond){
		WriteTimestampedLogEntry(sFunction + ":" + IntToString(nLine) + ": Assert failed" + (sMessage != "" ? ": " + sMessage : ""));
		SetLocalInt(GetModule(), "shutting_down", TRUE);
	}
}
void AssertEqS(string sA, string sB, string sFunction, int nLine, string sMessage=""){
	if(sA != sB){
		WriteTimestampedLogEntry(sFunction + ":" + IntToString(nLine) + ": Assert failed: '" + sA + "' != '" + sB + "'" + (sMessage != "" ? ": " + sMessage : ""));
		SetLocalInt(GetModule(), "shutting_down", TRUE);
	}
}
void AssertEqI(int nA, int nB, string sFunction, int nLine, string sMessage=""){
	if(nA != nB){
		WriteTimestampedLogEntry(sFunction + ":" + IntToString(nLine) + ": Assert failed: " + IntToString(nA) + " != " + IntToString(nB) + (sMessage != "" ? ": " + sMessage : ""));
		SetLocalInt(GetModule(), "shutting_down", TRUE);
	}
}
void AssertEqF(float fA, float fB, float fPrecision, string sFunction, int nLine, string sMessage=""){
	if(fabs(fA - fB) > fPrecision){
		WriteTimestampedLogEntry(sFunction + ":" + IntToString(nLine) + ": Assert failed: " + FloatToString(fA, 0) + " != " + FloatToString(fA, 0) + " (precision: " + FloatToString(fPrecision, 0) + ")" + (sMessage != "" ? ": " + sMessage : ""));
		SetLocalInt(GetModule(), "shutting_down", TRUE);
	}
}
//===================================================================





void xp_sql(){
	StartTimer(oModule, "nwnxtest_sqlperf");

	SQLCreateTables();

	// Simple var storage
	SetPersistentString(oModule, "nwnxtest_string", "Café");
	AssertEqS(GetPersistentString(oModule, "nwnxtest_string"), "Café", __FUNCTION__, __LINE__);
	SetPersistentInt(oModule, "nwnxtest_int", 42);
	AssertEqI(GetPersistentInt(oModule, "nwnxtest_int"), 42, __FUNCTION__, __LINE__);
	SetPersistentFloat(oModule, "nwnxtest_float", 13.37);
	AssertEqF(GetPersistentFloat(oModule, "nwnxtest_float"), 13.37, 0.0, __FUNCTION__, __LINE__);
	SetPersistentVector(oModule, "nwnxtest_vector", Vector(1.0, 2.0, 3.0));
	Assert(GetPersistentVector(oModule, "nwnxtest_vector") == Vector(1.0, 2.0, 3.0), __FUNCTION__, __LINE__);
	SetPersistentLocation(oModule, "nwnxtest_location", lStart);
	Assert(GetPersistentLocation(oModule, "nwnxtest_location") == lStart, __FUNCTION__, __LINE__);

	// SCORCO
	object oItem = CreateItemOnObject("nw_wswss001", oContainer);
	Assert(GetIsObjectValid(oItem), __FUNCTION__, __LINE__);
	SetPersistentObject(oModule, "nwnxtest_object", oItem);
	object oRetrieved = GetPersistentObject(oModule, "nwnxtest_object", oContainer);
	Assert(GetIsObjectValid(oRetrieved), __FUNCTION__, __LINE__);
	AssertEqS(GetName(oRetrieved), GetName(oItem), __FUNCTION__, __LINE__);
	DestroyObject(oItem);
	DestroyObject(oRetrieved);

	// Passthrough conventional campaign functions
	SetCampaignString("nwnxtest_cam", "string", "Café");
	AssertEqS(GetCampaignString("nwnxtest_cam", "string"), "Café", __FUNCTION__, __LINE__);
	SetCampaignInt("nwnxtest_cam", "int", 42);
	AssertEqI(GetCampaignInt("nwnxtest_cam", "int"), 42, __FUNCTION__, __LINE__);
	SetCampaignFloat("nwnxtest_cam", "float", 13.37);
	AssertEqF(GetCampaignFloat("nwnxtest_cam", "float"), 13.37, 0.0, __FUNCTION__, __LINE__);
	SetCampaignVector("nwnxtest_cam", "vector", Vector(1.0, 2.0, 3.0));
	Assert(GetCampaignVector("nwnxtest_cam", "vector") == Vector(1.0, 2.0, 3.0), __FUNCTION__, __LINE__);
	SetCampaignLocation("nwnxtest_cam", "location", lStart);
	Assert(GetCampaignLocation("nwnxtest_cam", "location") == lStart, __FUNCTION__, __LINE__);

	oItem = CreateItemOnObject("nw_wswss001", oContainer);
	Assert(GetIsObjectValid(oItem), __FUNCTION__, __LINE__);
	StoreCampaignObject("nwnxtest_cam", "object", oItem);
	oRetrieved = RetrieveCampaignObject("nwnxtest_cam", "object", GetLocation(oContainer), oContainer);
	Assert(GetIsObjectValid(oRetrieved), __FUNCTION__, __LINE__);
	AssertEqS(GetName(oRetrieved), GetName(oItem), __FUNCTION__, __LINE__);
	DestroyObject(oItem);
	DestroyObject(oRetrieved);

	WriteTimestampedLogEntry("[PERF] " + NWNXGetPluginSubClass("SQL") + ": " + FloatToString(StringToFloat(QueryTimer(oModule, "nwnxtest_sqlperf")) / 1000.0, 0, 2) + "ms");
}

void xp_time(){
	int nPluginIndex = NWNXFindPluginByClass("TIME");
	AssertEqS(NWNXGetPluginSubClass("TIME"), "TIME", __FUNCTION__, __LINE__);

	AssertEqS(NWNXGetPluginInfo(nPluginIndex), NWNXGetPluginDescription("TIME"), __FUNCTION__, __LINE__);
	AssertEqS(NWNXGetPluginSemVer(nPluginIndex), "0.0.2", __FUNCTION__, __LINE__);
	AssertEqS(NWNXGetPluginSemVer(nPluginIndex), NWNXGetPluginVersion("TIME"), __FUNCTION__, __LINE__);

	AssertEqS(QueryTimer(oModule, "nwnxtest_timer"), "0", __FUNCTION__, __LINE__);
	StartTimer(oModule, "nwnxtest_timer");
	DelayCommand(1.0, AssertEqF(StringToFloat(QueryTimer(oModule, "nwnxtest_timer")) / 1000000.0, 1.0, 0.1, __FUNCTION__, __LINE__));
	DelayCommand(1.5, AssertEqF(StringToFloat(QueryTimer(oModule, "nwnxtest_timer")) / 1000000.0, 1.5, 0.1, __FUNCTION__, __LINE__));
	DelayCommand(2.0, AssertEqF(StringToFloat(StopTimer(oModule, "nwnxtest_timer")) / 1000000.0, 2.0, 0.1, __FUNCTION__, __LINE__));
	DelayCommand(2.5, AssertEqS(QueryTimer(oModule, "nwnxtest_timer"), "0", __FUNCTION__, __LINE__));
}

void xp_funcs(){
	object oCreature = CreateObject(OBJECT_TYPE_CREATURE, "c_bear", lStart);
	Assert(GetIsObjectValid(oCreature), __FUNCTION__, __LINE__);

	AssertEqI(GetCreatureSoundSet(oCreature), 7, __FUNCTION__, __LINE__);

	DestroyObject(oCreature);
}

void xp_example_cplugin(){
	int nCount = NWNXGetPluginCount();
	Assert(nCount > 0, __FUNCTION__, __LINE__);

	int nPlugin = NWNXFindPluginByClass("CPluginExample");
	AssertEqS(NWNXGetPluginClass(nPlugin), "CPluginExample", __FUNCTION__, __LINE__);
	struct NWNXVersion version = NWNXExtractVersionNumbers(NWNXGetPluginSemVer(nPlugin));
	AssertEqI(version.major, 1, __FUNCTION__, __LINE__);
	AssertEqI(version.minor, 0, __FUNCTION__, __LINE__);
	AssertEqI(version.patch, 0, __FUNCTION__, __LINE__);

	AssertEqS(NWNXGetPluginInfo(nPlugin), "NWNX4 CPlugin Example - A C++ plugin showcasing C plugin ABI", __FUNCTION__, __LINE__);

	AssertEqI(NWNXGetInt("CPluginExample", "GET_COUNTER", "", 0), 0, __FUNCTION__, __LINE__);
	NWNXSetInt("CPluginExample", "SET_COUNTER", "", 0, 42);
	AssertEqI(NWNXGetInt("CPluginExample", "GET_COUNTER", "", 0), 42, __FUNCTION__, __LINE__);
	AssertEqI(NWNXGetInt("CPluginExample", "INC_COUNTER", "", 0), 43, __FUNCTION__, __LINE__);
	AssertEqI(NWNXGetInt("CPluginExample", "INC_COUNTER", "", 0), 44, __FUNCTION__, __LINE__);

	AssertEqF(NWNXGetFloat("CPluginExample", "", "", 0), 0.0, 0.0, __FUNCTION__, __LINE__);
	NWNXSetFloat("CPluginExample", "", "", 0, 13.37);
	AssertEqF(NWNXGetFloat("CPluginExample", "", "", 0), 13.37, 0.0, __FUNCTION__, __LINE__);

	AssertEqS(NWNXGetString("CPluginExample", "", "", 0), "", __FUNCTION__, __LINE__);
	NWNXSetString("CPluginExample", "", "", 0, "Hello world");
	AssertEqS(NWNXGetString("CPluginExample", "", "", 0), "Hello world", __FUNCTION__, __LINE__);
}

void nwnx_general(){
	int nCount = NWNXGetPluginCount();
	Assert(nCount > 0, __FUNCTION__, __LINE__);
	WriteTimestampedLogEntry("[INFO] Plugin count: " + IntToString(nCount));

	int i;
	for(i = 0 ; i < nCount ; i++)
	{
		string sClass = NWNXGetPluginClass(i);
		Assert(NWNXFindPluginByClass(sClass) == i, __FUNCTION__, __LINE__, "plugin index " + IntToString(i));

		if(sClass == "SQL"){
			// This is not a CPlugin, and it implements get version and get description
			Assert(NWNXGetPluginSubClass("SQL") == "MySQL" || NWNXGetPluginSubClass("SQL") == "SQLite", __FUNCTION__, __LINE__);
			AssertEqS(NWNXGetPluginInfo(i), NWNXGetPluginDescription(sClass), __FUNCTION__, __LINE__);
			Assert(NWNXGetPluginInfo(i) != "", __FUNCTION__, __LINE__);
			AssertEqS(NWNXGetPluginInfo(i), NWNXGetPluginDescription(sClass), __FUNCTION__, __LINE__);
			Assert(NWNXGetPluginSemVer(i) != "", __FUNCTION__, __LINE__);
			AssertEqS(NWNXGetPluginSemVer(i), NWNXGetPluginVersion(sClass), __FUNCTION__, __LINE__);
		}
	}

	Assert(NWNXGetPluginClass(i) == "", __FUNCTION__, __LINE__);
	Assert(NWNXGetPluginInfo(i) == "", __FUNCTION__, __LINE__);
}




void main()
{
	DelayCommand(6.0, ShutdownIfFailed());

	WriteTimestampedLogEntry("Execute " + __FILE__ + "================================");

	Assert(GetIsObjectValid(oContainer), __FUNCTION__, __LINE__);

	// Unit tests
	nwnx_general();
	xp_sql();
	DelayCommand(1.0, xp_time());
	xp_funcs();
	xp_example_cplugin();

	WriteTimestampedLogEntry("Finished " + __FILE__ + "================================");
}