#pragma once
extern "C"
{
	void _cdecl __vcrt_initialize_telemetry_provider() {}
	void _cdecl __telemetry_main_invoke_trigger() {}
	void _cdecl __telemetry_main_return_trigger() {}
	void _cdecl __vcrt_uninitialize_telemetry_provider() {}
};