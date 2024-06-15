<script>
    import { onMount } from 'svelte';

    let Module = {};
    let gameDataJson = '';

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
    onMount(async () => {
      try {
        const wasmUrl = '/c/test.wasm';
        console.log('Fetching WebAssembly module from:', wasmUrl);
        const wasmResponse = await fetch(wasmUrl);
        const { instance } = await WebAssembly.instantiateStreaming(wasmResponse, importObject);
        console.log('WebAssembly module instantiated.');

        Module = instance.exports;

        console.log('WebAssembly module loaded and ready.');
        console.log('Exported objects from WebAssembly module:', Module); // 打印 Module 物件
      } catch (error) {
        console.error('Error loading WebAssembly module:', error);
      }

    if (Module.getGameDataAsJson) {
        const ptr = Module.getGameDataAsJson(); // 假設這個函數不需要參數或已經有參數準備好
        const jsonString = UTF8ToString(ptr);
        gameDataJson = jsonString;
        console.log('Game Data JSON:', jsonString);

        // 如果需要，這裡可以解析 JSON 字符串並在 Svelte 應用中使用
        const gameData = JSON.parse(jsonString);
        console.log('Parsed Game Data:', gameData);
    }



    });
    


</script>

<head>
    <link rel="stylesheet" href="src/routes/Test.css">
</head>
<main>
  <div class="content">

  </div>
</main>

<style>
  /* Add your styles here */
</style>