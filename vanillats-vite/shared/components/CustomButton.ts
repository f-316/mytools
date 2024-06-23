import { CustomElement } from './CustomElement';

type CustomEventDetail = { name: string; uid: number };
export type CustomButtonEvent = CustomEvent<CustomEventDetail>;

let customButtonUID = 0;
export class CustomButton extends CustomElement {
  #uid: number;
  #clickEvent: CustomButtonEvent;
  constructor(name: string = '') {
    // コンストラクターでは常に super を最初に呼び出してください
    super(CustomButton);

    // メンバの初期化
    this.#uid = customButtonUID++;
    console.log(this.#uid);
    this.#clickEvent = new CustomEvent('#click', { detail: { name, uid: this.#uid } });

    // セットアップ
    this.#setup();

    // セットアップが終わってから呼び出して下さい
    this._setInnerHTML();
    this._setStyle();
  }

  connectedCallback() {
    console.log('connectedCallback');
  }
  disconnectedCallback() {
    console.log('disconnectedCallback');
  }
  get uid() {
    return this.#uid;
  }

  /**
   * セットアップ
   */
  #setup() {
    // console.log('#setup');
    this._shadow.addEventListener('click', this.#onClick.bind(this));
  }

  #onClick() {
    this.dispatchEvent(this.#clickEvent);
  }

  /**
   * 内容をセットします。
   */
  _setInnerHTML() {
    this._contents.innerHTML = /* HTML */`
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
  _setStyle() {
    const backgroundColor = this.getAttribute('background-color');
    this._style.textContent = /* CSS */ `
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

  /**
   * 属性変更時
   */
  static observedAttributes = ['background-color'];
  set backgroundColor(val: string) {
    this.setAttribute('background-color', val);
  }
  get backgroundColor() {
    return this.getAttribute('background-color') ?? '';
  }
}
