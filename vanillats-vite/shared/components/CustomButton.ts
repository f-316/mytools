import { CustomElement } from './CustomElement';

export class CustomButton extends CustomElement {
  constructor() {
    // コンストラクターでは常に super を最初に呼び出してください
    super(CustomButton);

    // メンバのセットアップ

    // メンバのセットアップが終わってから呼び出して下さい
    this._setInnerHTML();
    this._setStyle();
  }

  /**
   * 内容をセットします。
   */
  _setInnerHTML() {
    this._contents.innerHTML = `
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
    this._style.textContent = `
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
    `;
  }

  /**
   * 属性変更時
   */
  static observedAttributes = ['background-color'];
  set backgroundColor(val: string) {
    this.setAttribute('background-color', val);
  }
}
