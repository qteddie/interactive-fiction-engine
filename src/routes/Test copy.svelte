<script>
    import { onMount } from 'svelte';

    let Module = {};
    let gameDataJson = '';

    function UTF8ToString(ptr) {
        // 直接從 importObject.env.memory 獲取 memory 對象
        const data = new Uint8Array(importObject.env.memory.buffer, ptr);
        let length = 0;
        while (data[length]) length++; // 計算字串的實際長度
        return new TextDecoder('utf-8').decode(data.subarray(0, length));
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
                // 簡單的記憶體拷貝實作，注意這裡需要從 importObject.env.memory 獲取 buffer
                new Uint8Array(importObject.env.memory.buffer).set(new Uint8Array(importObject.env.memory.buffer, src, num), dest);
            },
            emscripten_resize_heap: emscripten_resize_heap,
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

        if(Module.hello){
            const ptr = Module.hello();
            console.log('Hello:', UTF8ToString(ptr));
        }
      if (Module.getGameDataAsJson) {
            const ptr = Module.getGameDataAsJson(); // 假設這個函數不需要參數或已經有參數準備好
            console.log('Game data JSON pointer:', ptr);
            const jsonString = UTF8ToString(ptr);
            console.log('Game Data JSON:', jsonString);
            if (!jsonString.trim()) {
                console.error('Game data JSON is empty or invalid.');
                return; // 提前返回以避免解析空或無效的 JSON 字符串
            }
            gameDataJson = jsonString;
            console.log('Game Data JSON:', jsonString);

            // 解析 JSON 字符串並在 Svelte 應用中使用
            try {
                const gameData = JSON.parse(jsonString);
                console.log('Parsed Game Data:', gameData);
            } catch (parseError) {
                console.error('Error parsing game data JSON:', parseError);
            }
        }
    }  
    catch (error) {
        console.error('Error loading WebAssembly module:', error);
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