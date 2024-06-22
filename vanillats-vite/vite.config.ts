import { defineConfig } from 'vite';
import { resolve } from 'path';

// https://vitejs.dev/config/
export default defineConfig({
  resolve: {
    alias: {
      '@/assets': resolve(__dirname, './shared/assets'),
      '@/components': resolve(__dirname, './shared/components'),
      '@/lib': resolve(__dirname, './shared/lib'),
      '@/src': resolve(__dirname, './src'),
    },
  },
});
