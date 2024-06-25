import { toKebab } from '@/lib/string-tool';
import { CustomElement } from '@/components/CustomElement';

export class TheContents extends CustomElement {
  /**
   * カスタム要素を定義
   */
  static use() {
    CustomElement.use(toKebab('TheContents'), TheContents);
  }

  constructor() {
    // コンストラクターでは常に super を最初に呼び出してください
    super();
  }

  /**
   * 内容をセットします。
   */
  protected override templateHTML() {
    return /* HTML */ `
      <div class="contents">
        <div class="foo">Hello!</div>
      </div>
    `;
  }

  /**
   * スタイルをセットします。
   */
  protected override styleHTML() {
    return /* CSS */ `
      .contents {
        display: flex;
        align-items: center;
      }
    `;
  }
}
