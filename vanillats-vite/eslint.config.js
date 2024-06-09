import globals from 'globals';

// prettierとの競合を回避するプラグイン
import pluginEcPrettier from 'eslint-config-prettier';
// rulesの値がeslintと互換がないためコメントアウト
// 'off'とすべきところをfalseで設定されている
// import pluginTcPrettier from 'tslint-config-prettier' with {type: "json"};
// Import pluginScPrettier from 'stylelint-config-prettier';

// eslintのプラグイン
import pluginJs from '@eslint/js';
// eslintのtypescript版のプラグイン
import {
  configs as tsLintConfigs,
  parser as tsLintParser,
} from 'typescript-eslint';

export default [
  pluginJs.configs.recommended,
  ...tsLintConfigs.recommended,
  // Override the recommended config
  { ignores: ['dist', '*.config.js'] },
  {
    files: ['**/*.js', '**/*.ts'],
    languageOptions: {
      ecmaVersion: 2022,
      sourceType: 'module',
      parser: tsLintParser, // tsファイルのparseに必須
      globals: globals.browser,
    },
    rules: {
      ...pluginEcPrettier.rules,
      'id-length': ['error', { min: 4 }],
      'init-declarations': 'error',
      'no-unused-vars': 'warn',
      // ...pluginTcPrettier.rules,
      // ...pluginScPrettier.rules,
    },
  },
];
