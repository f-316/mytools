import { toKebab } from '@/lib/string-tool';
import { CustomButton } from '@/components/CustomButton';

export class TheContents extends HTMLElement {
  #shadow: ShadowRoot;
  #contents: HTMLDivElement;
  #style: HTMLStyleElement;
  #button: CustomButton;

  constructor() {
    // カスタム要素を定義
    const customElemName = toKebab(TheContents.name);
    if (!customElements.get(customElemName)) {
      customElements.define(customElemName, TheContents);
    }

    // コンストラクターでは常に super を最初に呼び出してください
    super();
    this.#shadow = this.attachShadow({ mode: 'open' });

    this.#button = new CustomButton();
    setTimeout(() => {
      console.log('button backgroundColor change');
      this.#button.backgroundColor = 'red';
    }, 2000);

    // ここに要素の機能を記述します
    this.#style = document.createElement('style');
    this.#contents = document.createElement('div');
    this.updateStyle();
    this.updateContents();

    this.#shadow.append(this.#style, this.#contents);
  }

  updateStyle() {
    const backgroundColor = this.getAttribute('background-color');
    this.#style.textContent = `
      .contents {
        display: flex;
        align-items: center;
        background-color: ${backgroundColor};
      }
    `;
  }

  updateContents() {
    this.#contents.innerHTML = `
      <div class="contents">
      </div>
      `;
    this.#button.innerHTML = `
      <span slot="a">A</span>
      <span slot="b">B</span>
    `;
    this.#contents.querySelector('.contents')!.append(this.#button);
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
