<script setup lang="ts">
type Props = {
    cnt: number
};

defineProps<Props>();

// ToDo: style, default

</script>

<template>
    <h1>Child</h1>
    <div v-text="cnt" />
    <div v-text="cnt" />
</template>