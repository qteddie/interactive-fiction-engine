<script>
    import { onMount } from 'svelte';


    let result = 0;


    onMount(async () => {
        const wasmUrl = '/c/add.wasm';
        const response = await fetch(wasmUrl);
        const { instance } = await WebAssembly.instantiateStreaming(response);
        console.log(instance.exports); 
        // 使用 instance.exports.add 或其他你的 wasm 模組提供的函數
        result = instance.exports.add(10, 7);
    });
    $: console.log(result);

    let gameData = null;

    fetch('/json/output_option2.json')
        .then(response => {
            if (!response.ok) {
                throw new Error(`HTTP error! status: ${response.status}`);
            }
            return response.json();
        })
        .then(data => {
            console.log(data);
            gameData = data;
        })
        .catch(error => {
            console.error('There was a problem with the fetch operation: ', error);
        });
</script>
<p>{result}</p>



{#if gameData}
<div>
    <h2>遊戲資料：</h2>
    <p>名稱：{gameData.name}</p>
    <p>作者：{gameData.author}</p>
    <p>玩家：{gameData.player.starter}</p>
    <p>玩家：{gameData.player.inventory}</p>

    <h3>場景：</h3>
    {#each Object.entries(gameData.scene) as [sceneKey, sceneValue]}
        <p>{sceneKey}</p>
        <p>{sceneValue.name}</p>
        <!-- <p>{gameData.scene[sceneKey].background}</p> -->
    {/each}

    <h3>角色：</h3>
    {#each Object.entries(gameData.character) as [characterKey, characterValue]}
        <p>{gameData.character[characterKey].name}</p>
    {/each}

    <h3>物品：</h3>
    {#each Object.entries(gameData.item) as [itemKey, itemValue]}
        <p>{itemValue.name}</p>
        <p>{itemValue.description}</p>
    {/each}

    <h3>事件：</h3>
    {#each Object.entries(gameData.event) as [eventKey, eventValue]}
    <p><b>{eventKey}</b></p>
    <p>{eventValue.scene}</p>
    <p>{eventValue.dialogue}</p>
    {/each}

    <h3>對話：</h3>
    {#each Object.entries(gameData.dialogue) as [dialogueKey, dialogueValue]}
        <p><b>{dialogueKey}</b></p>
        <p>{dialogueValue.character}</p>
        <p>{dialogueValue.text}</p>
        <p>option text: {dialogueValue.options.text}</p>
        <p>option next: {dialogueValue.options.next}</p>
        <p>option event: {dialogueValue.options.event}</p>
    {/each}

</div>
{:else}
<p>Loading...</p>
{/if}