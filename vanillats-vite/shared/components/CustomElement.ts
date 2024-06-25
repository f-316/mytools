/**
 * カスタム要素を利用するための抽象クラスです
 */
export abstract class CustomElement extends HTMLElement {
  /**
   * カスタム要素を定義
   * @param customElementName
   * @param customElementConstructor
   */
  static use(customElementName: string, customElementConstructor: CustomElementConstructor) {
    if (!customElements.get(customElementName)) {
      customElements.define(customElementName, customElementConstructor);
    }
  }

  private m_shadow: ShadowRoot;
  m_tempElem: HTMLTemplateElement;
  m_styleElem: HTMLStyleElement;
  constructor() {
    // コンストラクターでは常に super を最初に呼び出してください
    super();

    this.m_shadow = this.attachShadow({ mode: 'open' });

    // ここに要素の機能を記述します
    this.m_tempElem = document.createElement('template');
    this.m_styleElem = document.createElement('style');

    this.m_shadow.append(this.m_styleElem, this.m_tempElem);
  }

  protected get shadow() {
    return this.m_shadow;
  }

  protected updateShadowRoot() {
    this.updateStyle();

    this.m_tempElem.innerHTML = this.templateHTML();
    const tempClone = this.m_tempElem.content.cloneNode(true);
    this.m_shadow.append(tempClone);
  }

  protected updateStyle() {
    this.m_styleElem.textContent = this.styleHTML();
  }

  /**
   * 派生でテンプレートをセットしてください。
   */
  protected abstract templateHTML(): string;

  /**
   * 派生でスタイルをセットしてください。
   */
  protected abstract styleHTML(): string;

  protected connectedCallback() {
    // console.log(`${this.constructor.name} added to page.`);
    this.updateShadowRoot();
  }

  protected disconnectedCallback() {
    // console.log(`${this.constructor.name} removed from page.`);
  }

  protected adoptedCallback() {
    // console.log(`${this.constructor.name} moved to new page.`);
  }

  /**
   * カスタム要素の属性の1つが追加、削除、変更されるたびに呼び出されます。
   * どの属性の変更が通知されたかは、静的な get observedAttributes() メソッドで指定されます。
   * @param name
   * @param oldVal
   * @param newVal
   */
  protected attributeChangedCallback(
    // eslint-disable-next-line no-unused-vars, @typescript-eslint/no-unused-vars
    _name: string,
    // eslint-disable-next-line no-unused-vars, @typescript-eslint/no-unused-vars
    _oldVal: string | null,
    // eslint-disable-next-line no-unused-vars, @typescript-eslint/no-unused-vars
    _newVal: string | null
  ) {
    // console.log('attributeChangedCallback', name);
    this.updateStyle();
  }
}
