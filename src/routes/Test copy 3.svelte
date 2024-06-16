<script>
    import { onMount } from 'svelte';

    let Module = {};

    function stringToUTF8(str, ptr) {
        const memory = Module.memory || importObject.env.memory;
        const data = new Uint8Array(memory.buffer, ptr);
        for (let i = 0; i < str.length; i++) {
            data[i] = str.charCodeAt(i);
        }
        data[str.length] = 0;  // Null-terminate the string
    }

    function UTF8ToString(ptr) {
        const memory = Module.memory || importObject.env.memory;
        const data = new Uint8Array(memory.buffer, ptr);
        let length = 0;
        while (data[length]) length++;
        return new TextDecoder('utf-8').decode(data.subarray(0, length));
    }

    function emscripten_resize_heap(requestedSize) {
        throw new Error('Heap resize is not supported');
    }
    import { WASI } from '@wasmer/wasi';
    import { WasmFs } from '@wasmer/wasmfs';
    import { lowerI64Imports } from '@wasmer/wasm-transformer';



    function processAndLogString(inputString) {
        const inputPtr = Module.malloc(inputString.length + 1); // Allocate memory for string
        stringToUTF8(inputString, inputPtr);
        console.log('Input string pointer:', inputPtr);
        const ptr = Module.processData(inputPtr);
        console.log('Processed data:', UTF8ToString(ptr));
        Module.free(inputPtr);
    }

    function getNextString(inputString) {
        const inputPtr = Module.malloc(inputString.length + 1); // Allocate memory for string
        stringToUTF8(inputString, inputPtr);
        const ptr = Module.getNextEncounter(inputPtr);
        // const result = UTF8ToString(ptr);
        console.log('Next data:', UTF8ToString(ptr));
        Module.free(inputPtr);
        // return result;
    }

    onMount(async () => {
        const wasmUrl = '/c/test.wasm';
        const wasmResponse = await fetch(wasmUrl);
        const wasmArrayBuffer = await wasmResponse.arrayBuffer();
        const wasmBytes = new Uint8Array(wasmArrayBuffer);

        const loweredWasmBytes = await lowerI64Imports(wasmBytes);
        const wasmFs = new WasmFs();
        let wasi = new WASI({
            // Pre-open directories
            preopens: {
                '/': '/',
            },
            // Environment variables
            env: {},
            args: [],
            bindings: {
                ...WASI.defaultBindings,
                fs: wasmFs.fs,
            },
        });
        let stackPointer = 0;
        const importObject = {
            wasi_snapshot_preview1: wasi.wasiImport,
            env: {
                memory: new WebAssembly.Memory({ initial: 256, maximum: 2048 }),
                table: new WebAssembly.Table({ initial: 0, element: 'anyfunc' }),
                __wbindgen_add_to_stack_pointer: function(offset) {
                    stackPointer += offset; // 調整堆疊指針
                    return stackPointer; // 返回新的堆疊指針位置
                },
                emscripten_memcpy_js: function(dest, src, num) {
                    const mem = new Uint8Array(this.memory.buffer);
                    if (src + num > mem.length || dest + num > mem.length) {
                        throw new Error("Attempt to access memory outside buffer bounds");
                    }
                    mem.set(mem.subarray(src, src + num), dest);
                },
                emscripten_resize_heap: emscripten_resize_heap,
            },    
        };
        const { instance } = await WebAssembly.instantiate(loweredWasmBytes, importObject);
        Module = instance.exports;
        wasi.start(instance);

        if (!Module.memory) {
            Module.memory = importObject.env.memory;
        }
        
        console.log('WebAssembly module loaded and ready.');
        processAndLogString("28749817834");
        getNextString("{'text': '2e�!', 'next': 'dragon_encounter'}");

    });
</script>