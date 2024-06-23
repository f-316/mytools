class MyButton extends HTMLElement {
  #shadow: ShadowRoot;
  #button: HTMLButtonElement;
  #style: HTMLStyleElement;
  constructor() {
    // コンストラクターでは常に super を最初に呼び出してください
    super();
    this.#shadow = this.attachShadow({ mode: 'open' });
    // ここに要素の機能を記述します
    this.#style = document.createElement('style');
    this.#button = document.createElement('button');
    this.#shadow.append(this.#style, this.#button);
  }

  updateStyle() {
    console.log('updateStyle');
    const color = this.getAttribute('color');
    this.#style.textContent = `
      button {
        color: ${color};
        background-color: gray;
        padding: 15px 32px;
        border: 1px solid black;
      }
    `;
  }
  set text(txt: string) {
    this.#button.innerHTML = txt ?? '<slot></slot>';
  }
  updateContents() {
    const text = this.getAttribute('text');
    this.#button.innerHTML = text ?? '<slot></slot>';
  }
  static observedAttributes = ['color', 'text'];
  connectedCallback() {
    console.log('Custom square element added to page.');
  }

  disconnectedCallback() {
    console.log('Custom square element removed from page.');
  }

  adoptedCallback() {
    console.log('Custom square element moved to new page.');
  }

  /**
   * 属性変更時
   * @param name
   * @param oldVal
   * @param newVal
   */
  // eslint-disable-next-line no-unused-vars, @typescript-eslint/no-unused-vars
  attributeChangedCallback(name: string, _oldVal: string | null, _newVal: string | null) {
    console.log('Custom square element attributes changed.');

    switch (name) {
      case 'color':
        this.updateStyle();
        break;

      default:
        this.updateContents();
        break;
    }
  }
}

// カスタム要素を定義
const ret = customElements.get('my-button');
console.log(ret);
if (!ret) {
  customElements.define('my-button', MyButton);
}
export { MyButton };
