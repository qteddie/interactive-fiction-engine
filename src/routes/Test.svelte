<script>
    import { onMount } from 'svelte';

    let Module = {};
    let gameDataJson = '';

    function stringToUTF8(str, ptr) {
        const memory = Module.memory || importObject.env.memory;
        const data = new Uint8Array(memory.buffer, ptr);
        for (let i = 0; i < str.length; i++) {
            data[i] = str.charCodeAt(i);
        }
        data[str.length] = 0;  // Null-terminate the string
    }

    function UTF8ToString(ptr) {
        // Use the memory from Module directly if available, else use importObject's memory
        const memory = Module.memory || importObject.env.memory;
        const data = new Uint8Array(memory.buffer, ptr);
        let length = 0;
        while (data[length]) length++;  // Calculate the actual string length
        return new TextDecoder('utf-8').decode(data.subarray(0, length));
    }

    function emscripten_resize_heap(requestedSize) {
        throw new Error('Heap resize is not supported');
    }

    const importObject = {
        env: {
            // ...wasi.getImports().env,
            memory: new WebAssembly.Memory({ initial: 256, maximum: 512 }),
            table: new WebAssembly.Table({ initial: 0, element: 'anyfunc' }),
            __wbindgen_add_to_stack_pointer: () => {return 0; },
            emscripten_memcpy_js: function(dest, src, num) {
                new Uint8Array(importObject.env.memory.buffer).set(new Uint8Array(importObject.env.memory.buffer, src, num), dest);
            },
            emscripten_resize_heap: emscripten_resize_heap,

        },    
    };
    let gameData = null;
    onMount(async () => {
        // try {
            const wasmUrl = '/c/test.wasm';  // Ensure this path is correct
            const wasmResponse = await fetch(wasmUrl);
            const { instance } = await WebAssembly.instantiateStreaming(wasmResponse, importObject);

            Module = instance.exports;
            // Do not reassign; use directly
            if (!Module.memory) {
                Module.memory = importObject.env.memory;
            }
            
            console.log('WebAssembly module loaded and ready.');

            const response = await fetch('/json/output3.json');
            gameData = await response.json();
            console.log(gameData);
            const gameDataJsonString = JSON.stringify(gameData);
            // console.log(gameDataJsonString);

            // if(Module.processData)
            // {
            //     const ptr = Module.processData(gameDataJsonString);
            //     console.log('processData:', UTF8ToString(ptr));
            // } else {
            //     console.log('processData not found');
            // }

            // if (Module.initGame) {
            //     Module.initGame();
            // }
            // if (Module.hello) {
            //     const ptr = Module.hello("Hello from Svelte");
            //     console.log('Hello:', UTF8ToString(ptr));
            // }

            const inputString = "aaaa";
            const inputPtr = Module.malloc(gameDataJsonString.length + 1); // Allocate memory for string
            // const inputPtr = Module.malloc(inputString.length + 1); // Allocate memory for string
            stringToUTF8(inputString, inputPtr);
            console.log('Input string pointer:', inputPtr);
            const ptr = Module.hello(inputPtr);
            console.log('Hello:', UTF8ToString(ptr));

            Module.free(inputPtr); 

            // if (Module.getGameDataAsJson) {
            //     const ptr = Module.getGameDataAsJson();
            //     console.log('Game data JSON pointer:', ptr);
            //     const jsonString = UTF8ToString(ptr);
            //     console.log('Game Data JSON:', jsonString);
            //     if (!jsonString.trim()) {
            //         console.error('Game data JSON is empty or invalid.');
            //         return;
            //     }
            //     gameDataJson = jsonString;
            //     console.log('Game Data JSON:', jsonString);
            // }
        // } catch (error) {
        //     console.error('Error loading WebAssembly module:', error);
        // }
    });
</script>