emcc Plugin_wasm.cpp -o Plugin.js -s FETCH=1 -s EXPORTED_FUNCTIONS=_CheckLicensePlugin,_GetLicenseCheckResult -s EXPORTED_RUNTIME_METHODS=ccall,cwrap

