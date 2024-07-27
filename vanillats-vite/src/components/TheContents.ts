import { toKebab } from '@/lib/string-tool';
import { CustomElement } from '@/components/CustomElement';
import { CustomButton } from '@/components/CustomButton';
CustomButton.use();

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

  protected connectedCallback() {
    super.connectedCallback();

    const button1 = this.shadow.querySelector('custom-button[name="1"]');
    setTimeout(() => {
      button1!.remove();
    }, 3000);
  }

  /**
   * 内容をセットします。
   */
  protected override templateHTML() {
    return /* HTML */ `
      <div class="contents">
        <div class="foo">Hello!</div>
        <custom-button class="foo" name="1">
          Hello!
          <div slot="caption">aaa</div>
          <div slot="icon">iii</div>
        </custom-button>
        <custom-button class="foo" name="2">Hello!</custom-button>
      </div>
    `;
  }

  /**
   * スタイルをセットします。
   */
  protected override styleHTML() {
    return /* CSS */ `
      .contents {
        padding: 12px;
      }
    `;
  }
}
