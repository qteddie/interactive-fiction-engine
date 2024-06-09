<script>
    import { onMount } from 'svelte';
    import { music } from './store.js';
    let volume = localStorage.getItem('volume') || 0.5; // 從 localStorage 讀取音量，如果不存在則預設為 0.5

    onMount(() => {
        // 更新所有音樂的音量
        music.update(musicArray => {
            for (let i = 0; i < musicArray.length; i++) {
                musicArray[i].volume = volume;
            }
            return musicArray;
        });
    });

    // 當滑塊的值改變時，更新音量
    function handleVolumeChange(event) {
        volume = event.target.value;
        // 更新所有音樂的音量
        music.update(musicArray => {
            for (let i = 0; i < musicArray.length; i++) {
                musicArray[i].volume = volume;
            }
            return musicArray;
        });
        // 將音量儲存到 localStorage
        localStorage.setItem('volume', volume);
    }
</script>

<head>
    <link rel="stylesheet" href="src/routes/setting.css">
</head>
<div class="content">
    <h1>Setting</h1>
    <div>
        <label for="volume">音樂音量：</label>
        <input type="range" id="volume" min="0" max="1" step="0.01" value={volume} on:input={handleVolumeChange}>
    </div>
</div>