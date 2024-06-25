import { toKebab } from '@/lib/string-tool';
import { CustomElement } from './CustomElement';

type CustomEventDetail = { name: string; uid: number };
export type CustomButtonEvent = CustomEvent<CustomEventDetail>;

let customButtonUID = 0;
export class CustomButton extends CustomElement {
  /**
   * カスタム要素を定義
   */
  static use() {
    CustomElement.use(toKebab('CustomButton'), CustomButton);
  }

  protected m_name: string;
  protected m_uid: number;
  constructor(name: string = '') {
    // コンストラクターでは常に super を最初に呼び出してください
    super();

    // メンバの初期化
    this.m_name = name;
    this.m_uid = customButtonUID++;

    this.shadow.addEventListener('click', this.#onClick.bind(this));
  }

  connectedCallback() {
    console.log('connectedCallback');
  }
  disconnectedCallback() {
    console.log('disconnectedCallback');
  }
  adoptedCallback() {
    console.log('adoptedCallback');
  }

  getUid() {
    return this.m_uid;
  }

  #onClick() {
    const clickEvent = new CustomEvent('custom-click', {
      detail: { name: this.m_name, uid: this.m_uid },
    });
    this.dispatchEvent(clickEvent);
  }

  /**
   * 内容をセットします。
   */
  protected override templateHTML() {
    return /* HTML */ `
      <div class="contents">
        <div class="icon">
          <slot name="icon"></slot>
        </div>
        <div class="caption">
          <slot name="caption"></slot>
        </div>
      </div>
    `;
  }

  /**
   * スタイルをセットします。
   */
  protected override styleHTML() {
    const backgroundColor = this.getAttribute('background-color');
    return /* CSS */ `
      .contents {
        user-select: none;
        display: flex;
        align-items: center;
        justify-content: center;
        flex-direction: column;
        min-width: 5rem;
        min-height: 2rem;
        border: 1px white solid;
        background-color: ${backgroundColor};
      }

      :active {
        opacity: 0.4;
      }
    `;
  }
}
