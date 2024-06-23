import { CustomElement } from './CustomElement';

type CustomEventDetail = { name: string; uid: number };
export type CustomWindowEvent = CustomEvent<CustomEventDetail>;

let customWindowUID = 0;
export class CustomWindow extends CustomElement {
  #uid: number;
  #windowPos: { x: number; y: number };
  #lastPagePos: { x: number; y: number };
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
    this.#lastPagePos = { x: 0, y: 0 };
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
    this._contents.innerHTML = /* HTML */ `
      <div class="backdrop">
        <div class="frame"></div>
      </div>
    `;
    const frame = this._shadow.querySelector('.frame');
    frame!.append(this.#title, this.#body);

    this.#title!.addEventListener('pointerdown', this.#onPointerDown.bind(this));
    this.#title!.addEventListener('pointerup', this.#onPointerUP.bind(this));
    this.#title!.addEventListener('pointerleave', this.#onPointerLeave.bind(this));
    window.addEventListener('pointermove', this.#onPointerMove.bind(this));
  }

  #onPointerDown(event: PointerEvent) {
    this.#lastPagePos.x = event.x;
    this.#lastPagePos.y = event.y;
    this.#isDown = true;
  }

  #onPointerLeave() {
    if (!this.#isDown) return;
    this.#isDown = true;
  }

  #onPointerUP() {
    this.#isDown = false;
    // console.log(event);
  }

  #onPointerMove(event: PointerEvent) {
    if (!this.#isDown) return;
    const diffX = event.x - this.#lastPagePos.x;
    const diffY = event.y - this.#lastPagePos.y;
    this.#lastPagePos.x = event.x;
    this.#lastPagePos.y = event.y;
    this.#windowPos.x += diffX;
    this.#windowPos.y += diffY;
    this._setStyle();
  }

  /**
   * スタイルをセットします。
   */
  _setStyle() {
    const backgroundColor = this.getAttribute('background-color');
    this._style.textContent = /* CSS */ `
      .backdrop {
        pointer-events: none;
        width: 100%;
        height: 100%;
        position: fixed;
        left: 0;
        top: 0;
      }

      .frame {
        pointer-events: auto;
        position: absolute;
        left: ${this.#windowPos.x}px;
        top: ${this.#windowPos.y}px;
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
        width: 100%;
        height: 2rem;
        background-color: orange;
      }

      .body {
        height: 5rem;
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
