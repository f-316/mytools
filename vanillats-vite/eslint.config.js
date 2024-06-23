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
import { configs as tsLintConfigs, parser as tsLintParser } from 'typescript-eslint';

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
      'id-length': ['error', { min: 1 }],
      'init-declarations': 'error',
      'no-unused-vars': 'warn',
      // コンソールは許可
      'no-console': 'off',
      // varは禁止
      'no-var': 'error',
      // 文字列はシングルクオートのみ,テンプレートリテラルは許可
      quotes: ['error', 'single', { allowTemplateLiterals: true }],
      // 不要なカッコは消す
      'no-extra-parens': 'error',
      // 無駄なスペースは削除
      'no-multi-spaces': 'error',
      // 不要な空白行は削除。2行開けてたらエラー
      'no-multiple-empty-lines': ['error', { max: 1 }],
      // ...pluginTcPrettier.rules,
      // ...pluginScPrettier.rules,
    },
  },
];
