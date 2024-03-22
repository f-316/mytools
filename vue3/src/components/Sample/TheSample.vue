<script setup lang="ts">
import {ref} from 'vue'
import {TheChild} from './Child'

const cnt = ref(0);
setInterval(() => {
  cnt.value++
}, 1000)

</script>

<template>
    <TheChild :cnt="cnt"/>
</template>