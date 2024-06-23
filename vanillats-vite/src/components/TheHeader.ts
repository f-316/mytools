import typescriptLogo from '@/assets/typescript.svg';
import viteLogo from '@/assets/vite.svg';
import { toKebab } from '@/lib/string-tool';

export class TheHeader extends HTMLElement {
  #shadow: ShadowRoot;
  #contents: HTMLDivElement;
  #style: HTMLStyleElement;

  constructor() {
    // カスタム要素を定義
    const customElemName = toKebab(TheHeader.name);
    if (!customElements.get(customElemName)) {
      customElements.define(customElemName, TheHeader);
    }

    // コンストラクターでは常に super を最初に呼び出してください
    super();
    this.#shadow = this.attachShadow({ mode: 'open' });

    // ここに要素の機能を記述します
    this.#style = document.createElement('style');
    this.#contents = document.createElement('div');
    this.updateStyle();
    this.updateContents();

    this.#shadow.append(this.#style, this.#contents);
  }

  updateStyle() {
    const backgroundColor = this.getAttribute('background-color');
    this.#style.textContent = /* CSS */ `
      .header {
        display: flex;
        align-items: center;
        background-color: ${backgroundColor};
      }
    `;
  }

  updateContents() {
    this.#contents.innerHTML = /* HTML */ `
      <div class="header">
        <a href="https://vitejs.dev" target="_blank">
          <img src="${viteLogo}" class="logo" alt="Vite logo" />
        </a>
        <a href="https://www.typescriptlang.org/" target="_blank">
          <img src="${typescriptLogo}" class="logo vanilla" alt="TypeScript logo" />
        </a>
        <h1 style="margin-left: 2rem;">Vite + TypeScript</h1>
      </div>
    `;
  }
  connectedCallback() {
    // console.log(`${this.constructor.name} added to page.`);
  }

  disconnectedCallback() {
    // console.log(`${this.constructor.name} removed from page.`);
  }

  adoptedCallback() {
    // console.log(`${this.constructor.name} moved to new page.`);
  }

  /**
   * 属性変更時
   * @param name
   * @param oldVal
   * @param newVal
   */
  static observedAttributes = ['background-color', 'text'];
  set backgroundColor(val: string) {
    this.setAttribute('background-color', val);
  }
  // eslint-disable-next-line no-unused-vars, @typescript-eslint/no-unused-vars
  attributeChangedCallback(name: string, _oldVal: string | null, _newVal: string | null) {
    // console.log(`${this.constructor.name} attributes changed.`);

    switch (name) {
      case 'background-color':
        this.updateStyle();
        break;

      default:
        this.updateContents();
        break;
    }
  }
}
