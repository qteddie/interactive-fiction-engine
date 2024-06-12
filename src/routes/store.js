import { writable } from 'svelte/store';

export const gameState = writable(null);
export const loadedGameState = writable(null);
export const music = writable([]);