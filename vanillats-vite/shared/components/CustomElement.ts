import { toKebab } from '@/lib/string-tool';

/**
 * カスタム要素を利用するための抽象クラスです
 */
export abstract class CustomElement extends HTMLElement {
  _shadow: ShadowRoot;
  _contents: HTMLDivElement;
  _style: HTMLStyleElement;
  _customElementClass: CustomElementConstructor;
  _customElementName: string;
  constructor(customElementClass: CustomElementConstructor) {
    // カスタム要素を定義
    const customElemName = toKebab(customElementClass.name);
    if (!customElements.get(customElemName)) {
      customElements.define(customElemName, customElementClass);
    }

    // コンストラクターでは常に super を最初に呼び出してください
    super();

    this._customElementClass = customElementClass;
    this._customElementName = customElemName;
    this._shadow = this.attachShadow({ mode: 'open' });

    // ここに要素の機能を記述します
    this._contents = document.createElement('div');
    this._style = document.createElement('style');

    // 継承先でセットしてください。
    // this._setInnerHTML();
    // this._setStyle();

    this._shadow.append(this._style, this._contents);
  }

  /**
   * 内容をセットします。
   */
  _setInnerHTML() {
    this._contents.innerHTML = ``;
  }

  /**
   * スタイルをセットします。
   */
  _setStyle() {
    this._style.textContent = ``;
  }

  connectedCallback() {
    // console.log(`${this.constructor.name} added to page.`);
    const customElems = document.querySelectorAll(this._customElementName);
    console.log(customElems);
  }

  disconnectedCallback() {
    // console.log(`${this.constructor.name} removed from page.`);
  }

  adoptedCallback() {
    // console.log(`${this.constructor.name} moved to new page.`);
  }

  /**
   * カスタム要素の属性の1つが追加、削除、変更されるたびに呼び出されます。
   * どの属性の変更が通知されたかは、静的な get observedAttributes() メソッドで指定されます。
   * @param name
   * @param oldVal
   * @param newVal
   */
  // eslint-disable-next-line no-unused-vars, @typescript-eslint/no-unused-vars
  attributeChangedCallback(_name: string, _oldVal: string | null, _newVal: string | null) {
    // console.log('attributeChangedCallback', name);
    this._setStyle();
  }
}
