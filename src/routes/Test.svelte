<script>
    import { onMount } from 'svelte';

    let randomStr = ''; // 反應式變量來存儲隨機字符串
    let cbind = {}; // 導出 WebAssembly 模塊的對象
    onMount(async () => {
        const wasmUrl = '/c/test6.wasm';
        const cbind = await fetch(wasmUrl);; // 初始化並獲取 WebAssembly 模塊的導出
        const buffer = cbind.malloc(100); // 分配內存
        const r = cbind.getRandomStr(buffer); // 獲取隨機字符串的長度
        console.log('len is', r);
        randomStr = cbind.UTF8ToString(buffer); // 將內存中的字符串轉換為 JS 字符串
        console.log(randomStr);
        cbind.free(buffer); // 釋放內存
    });
</script>

<h1>Random String: {randomStr}</h1>