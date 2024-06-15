<script>
    import { onMount } from 'svelte';
  
    let currentDialogueText = '';
    let gameData = {
      dialogue: [
        { "next": "Hello, this is the first dialogue." },
        { "next": "This is the second dialogue." },
      ],
    };
  
    let Module = {};
    
    function UTF8ToString(ptr) {
        const data = new Uint8Array(Module.memory.buffer, ptr);
        let str = "";
        for (let i = 0; data[i]; i++) {
            str += String.fromCharCode(data[i]);
        }
        return decodeURIComponent(escape(str));
    }

    function emscripten_resize_heap(requestedSize) {
        // 假設你的記憶體是固定大小的，可以拋出一個錯誤，或根據需求實作記憶體擴充邏輯
        throw new Error('Heap resize is not supported');
    }
    onMount(async () => {
      try {
        const wasmUrl = '/c/main.wasm';
        const importObject = {
            env: {
                memory: new WebAssembly.Memory({ initial: 32, maximum: 64 }),
                table: new WebAssembly.Table({ initial: 0, element: 'anyfunc' }),
                emscripten_memcpy_js: function(dest, src, num) {
                    // 簡單的記憶體拷貝實作
                    new Uint8Array(memory.buffer).set(new Uint8Array(memory.buffer, src, num), dest);
                },
                emscripten_resize_heap:  emscripten_resize_heap,

            },
        };

        console.log('Fetching WebAssembly module from:', wasmUrl);
        const wasmResponse = await fetch(wasmUrl);
        const { instance } = await WebAssembly.instantiateStreaming(wasmResponse, importObject);
        console.log('WebAssembly module instantiated.');

        Module = instance.exports;
        console.log('WebAssembly module loaded and ready.');
      } catch (error) {
        console.error('Error loading WebAssembly module:', error);
      }
    });
    

    async function handleNextDialogue(optionNext) {
        await Module.ready; // Ensure the module is loaded

        const gameDataJson = JSON.stringify(gameData);
        const resultPointer = Module.get_game_data_response(gameDataJson, optionNext);
        
        const currentDialogueText = UTF8ToString(resultPointer);
        console.log(JSON.parse(currentTimeDialogueText)); // Output the parsed JSON object

        Module.free_memory(resultPointer); // Free the allocated memory
    }
</script>

<head>
    <link rel="stylesheet" href="src/routes/Test.css">
</head>
<main>
  <div class="content">
    <p>{currentDialogueText}</p>
    <button on:click={() => handleNextDialogue('dialogue1')}>Next Dialogue 1</button>
    <button on:click={() => handleNextDialogue('dialogue2')}>Next Dialogue 2</button>
  </div>
</main>

<style>
  /* Add your styles here */
</style>