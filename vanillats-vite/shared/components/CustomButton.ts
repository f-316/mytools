import { CustomElement } from './CustomElement';

export class CustomButton extends CustomElement {
  constructor() {
    // コンストラクターでは常に super を最初に呼び出してください
    super(CustomButton);

    // ここに要素の機能を記述します
  }

  /**
   * スタイルをセットします。
   */
  _setStyle() {
    const backgroundColor = this.getAttribute('background-color');
    this._style.textContent = `
      .contents {
        display: flex;
        align-items: center;
        background-color: ${backgroundColor};
      }
    `;
  }

  _setInnerHTML() {
    this._contents.innerHTML = `
      <div class="contents">
        <slot name="a"></slot>
      </div>
      <div class="contents">
        <slot name="b"></slot>
      </div>
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
