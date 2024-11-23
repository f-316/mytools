import { defineConfig } from 'vite';

export default defineConfig({
    build: {
        lib: {
            name: 'MyLib',
            entry: {
                "Lib1": './lib/Lib1/index.ts',
                "Lib2": './lib/Lib2/index.ts',
                "Lib3": './lib/Lib3/index.ts'
            },
            formats: ['es'],
            fileName: (format, entryName) => `lib/${entryName}/index.js`,
        },
        rollupOptions: {
        },
        // minify: false,
    }
})