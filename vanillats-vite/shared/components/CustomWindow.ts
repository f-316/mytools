import { toKebab } from '@/lib/string-tool';
import { CustomElement } from './CustomElement';

type CustomEventDetail = { name: string; uid: number };
export type CustomWindowEvent = CustomEvent<CustomEventDetail>;

type Point = { x: number; y: number };

let customWindowUID = 0;
export class CustomWindow extends CustomElement {
  /**
   * カスタム要素を定義
   */
  static use() {
    CustomElement.use(toKebab('CustomWindow'), CustomWindow);
  }

  protected m_windowPos: Point;
  protected m_lastPagePos: Point | null = null;
  protected m_name: string;
  protected m_uid: number;
  constructor(name: string = '') {
    // コンストラクターでは常に super を最初に呼び出してください
    super();

    // メンバの初期化
    this.m_name = name;
    this.m_uid = customWindowUID++;
    this.m_windowPos = { x: 0, y: 0 };
  }

  connectedCallback() {
    console.log('connectedCallback');
    const titleElem = this.shadow.querySelector('.frame .title');
    if (titleElem) {
      titleElem.addEventListener('pointerdown', this.onPointerDown.bind(this) as EventListener);
      titleElem.addEventListener('pointerup', this.onPointerUP.bind(this));
      titleElem.addEventListener('pointerleave', this.onPointerUP.bind(this));
      window.addEventListener('pointermove', this.onPointerMove.bind(this));
    }
  }
  disconnectedCallback() {
    console.log('disconnectedCallback');
    const titleElem = this.shadow.querySelector('.frame .title');
    if (titleElem) {
      titleElem.removeEventListener('pointerdown', this.onPointerDown.bind(this) as EventListener);
      titleElem.removeEventListener('pointerup', this.onPointerUP.bind(this));
      titleElem.removeEventListener('pointerleave', this.onPointerUP.bind(this));
      window.removeEventListener('pointermove', this.onPointerMove.bind(this));
    }
  }
  getUid() {
    return this.m_uid;
  }

  /**
   * 内容をセットします。
   */
  protected override templateHTML(): string {
    return /* HTML */ `
      <div class="backdrop">
        <div class="frame">
          <div class="title">
            <slot name="title"></slot>
          </div>
          <div class="body">
            <slot name="body"></slot>
          </div>
        </div>
      </div>
    `;
  }

  onPointerDown(event: PointerEvent) {
    this.m_lastPagePos = { x: event.x, y: event.y };
  }

  onPointerUP() {
    this.m_lastPagePos = null;
  }

  onPointerMove(event: PointerEvent) {
    if (!this.m_lastPagePos) return;
    const diffX = event.x - this.m_lastPagePos.x;
    const diffY = event.y - this.m_lastPagePos.y;
    this.m_lastPagePos.x = event.x;
    this.m_lastPagePos.y = event.y;
    this.m_windowPos.x += diffX;
    this.m_windowPos.y += diffY;
    this.updateStyle();
  }

  /**
   * スタイルをセットします。
   */
  static observedAttributes = ['background-color'];
  protected styleHTML() {
    const backgroundColor = this.getAttribute('background-color');
    return /* CSS */ `
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
        left: ${this.m_windowPos.x}px;
        top: ${this.m_windowPos.y}px;
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
}
