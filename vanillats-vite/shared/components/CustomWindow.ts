import { CustomElement } from './CustomElement';

type CustomEventDetail = { name: string; uid: number };
export type CustomWindowEvent = CustomEvent<CustomEventDetail>;

let customWindowUID = 0;
export class CustomWindow extends CustomElement {
  #uid: number;
  #windowPos: { x: number; y: number };
  #isDown = false;
  #title: HTMLDivElement;
  #body: HTMLDivElement;
  constructor(name: string = '') {
    // コンストラクターでは常に super を最初に呼び出してください
    super(CustomWindow);

    // メンバの初期化
    this.#uid = customWindowUID++;
    console.log(name);
    console.log(this.#uid);
    this.#windowPos = { x: 0, y: 0 };
    this.#title = document.createElement('div');
    this.#body = document.createElement('div');

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
  }

  /**
   * 内容をセットします。
   */
  _setInnerHTML() {
    this.#title.innerHTML = /* HTML */ `
      <div class="title">
        <slot name="title"></slot>
      </div>
    `;
    this.#body.innerHTML = /* HTML */ `
      <div class="body">
        <slot name="body"></slot>
      </div>
    `;
    this._contents.innerHTML = /* HTML */ ` <div class="frame"></div> `;
    const frame = this._shadow.querySelector('.frame');
    frame!.append(this.#title, this.#body);

    this.#title!.addEventListener('pointerdown', this.#onPointerDown.bind(this));
    this.#title!.addEventListener('pointerup', this.#onPointerUP.bind(this));
    this.#title!.addEventListener('pointerleave', this.#onPointerUP.bind(this));
    this.#title!.addEventListener('pointerout', this.#onPointerUP.bind(this));
    this.#title!.addEventListener('pointermove', this.#onPointerMove.bind(this));
  }

  #onPointerDown(event: PointerEvent) {
    console.log(event);
    this.#isDown = true;
  }

  #onPointerUP(event: PointerEvent) {
    this.#isDown = false;
    // console.log(event);
  }

  #onPointerMove(event: PointerEvent) {
    if (!this.#isDown) return;
    this.#windowPos.x = event.pageX;
    this.#windowPos.y = event.pageY;
    this._setStyle();
    console.log(event);
    console.log(this.#windowPos);
  }

  /**
   * スタイルをセットします。
   */
  _setStyle() {
    const backgroundColor = this.getAttribute('background-color');
    this._style.textContent = /* CSS */ `
      .frame {
        position: absolute;
        left: ${this.#windowPos.x};
        top: ${this.#windowPos.y};
        user-select: none;
        display: flex;
        align-items: center;
        justify-content: center;
        flex-direction: column;
        border: 1px white solid;
        border-radius: 3px;
        background-color: ${backgroundColor};
      }

      .title {
        height: 1rem;
        background-color: orange;
      }

      .body {
        height: 2rem;
        background-color: green;
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
