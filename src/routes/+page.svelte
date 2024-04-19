<script>
    let scenes = [
        {
            id: 'classroom',
            description: 'You are in a dark room. There are two doors.',
            image: 'scenes/classroom.jpg',
            choices: [
                { text: 'Left door', nextScene: 'treasure' },
                { text: 'Right door', nextScene: 'pit' },
            ],
        },
        // 更多場景...
    ];

    let characters = [
        {
            id: 'player',
            name: 'Frieren',
            avatar: 'characters/frieren.png',
            items: [],
            // 更多角色屬性...
        },
        // 更多角色...
    ];

    let items = [
        {
            id: 'book',
            name: 'book',
            icon: 'items/book.jpeg',
        },
        {
            id: 'pen',
            name: 'pen',
            icon: 'items/pen.jpeg',
        },
        {
            id: 'laptop',
            name: 'laptop',
            icon: 'items/laptop.jpeg',
        },
        
    ];

    let currentScene = scenes[0];
    let player = characters.find(character => character.id === 'player');

    function makeChoice(choice) {
        currentScene = scenes.find(scene => scene.id === choice.nextScene);
    }
</script>

<head>
    <link rel="stylesheet" href="src/styles.css">
</head>

<div class="scene-container">
    <img class="scene-image" src={currentScene.image} alt={currentScene.description} />
    <img class="character-avatar" src={player.avatar} alt={player.name} />
</div>
<div class="subtitle-window">
    <h2>{player.name}</h2>
    <p>{currentScene.description}</p>
</div>
<div class="item-container">
    {#each items as item (item.id)}
        <div class="item">
            <img src={item.icon} alt={item.name} />
            <p>{item.name}</p>
        </div>
    {/each}
</div>