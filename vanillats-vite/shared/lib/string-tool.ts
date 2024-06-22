const UPPERCASE_PATTERN = /([A-Z])/g;
/**
 * ケバブケースに変換します。
 * @param src - 任意の文字列
 * @returns ケバブケース
 */
export function toKebab(src: string) {
  if (!src.match(UPPERCASE_PATTERN)) return src;
  return (src[0] + src.slice(1).replaceAll(UPPERCASE_PATTERN, `-${'$1'}`)).toLowerCase();
}

/**
 * スネークに変換します。
 * @param src - 任意の文字列
 * @returns スネークケース
 */
export function toSnake(src: string) {
  return (src[0] + src.slice(1).replaceAll(UPPERCASE_PATTERN, `_${'$1'}`)).toLowerCase();
}
