import { toKebab } from '@/lib/string-tool';
import { CustomElement } from './CustomElement';

type CustomEventDetail = { name: string; uid: number };
export type CustomButtonEvent = CustomEvent<CustomEventDetail>;

let customButtonUID = 0;
export class CustomButton extends CustomElement {
  /**
   * カスタム要素を定義
   */
  static use() {
    CustomElement.use(toKebab('CustomButton'), CustomButton);
  }

  protected m_name: string;
  protected m_uid: number;
  protected m_slots: HTMLSlotElement[] | undefined;
  protected m_id: string | undefined;
  constructor(name: string = '') {
    // コンストラクターでは常に super を最初に呼び出してください
    super();

    // メンバの初期化
    this.m_name = this.getAttribute('name') ?? name;
    this.m_uid = customButtonUID++;

    this.shadow.addEventListener('click', this.onClick.bind(this));
  }
  protected onEvent: EventListenerOrEventListenerObject | undefined;

  protected override connectedCallback() {
    this.updateShadowRoot();
    // console.log('connectedCallback');
    this.m_slots = [...this.shadow.querySelectorAll('slot')];
    if (!this.m_slots) return;
    this.m_slots.forEach((slot) => {
      this.onEvent = (event: Event) => {
        console.log(slot.name);
        console.log(event);
      };
      slot.addEventListener('slotchange', this.onEvent);
    });
  }

  protected override disconnectedCallback() {
    // console.log('disconnectedCallback');
    if (!this.m_slots) return;
    this.m_slots.forEach((slot) => {
      if (!this.onEvent) return;
      console.log(`${slot.name}, disconnected`);
      slot.removeEventListener('slotchange', this.onEvent);
    });
  }
  protected override adoptedCallback() {
    // console.log('adoptedCallback');
  }

  getUid() {
    return this.m_uid;
  }

  protected onClick() {
    const clickEvent = new CustomEvent('custom-click', {
      detail: { name: this.m_name, uid: this.m_uid },
    });
    this.dispatchEvent(clickEvent);
  }

  /**
   * 内容をセットします。
   */
  protected override templateHTML() {
    return /* HTML */ `
      <div class="contents">
        <slot></slot>
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
  protected override styleHTML() {
    const backgroundColor = this.getAttribute('background-color');
    return /* CSS */ `
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

      .icon {
        color: red;
      }
      
      .caption {
        color: green;
      }

      :active {
        opacity: 0.4;
      }
    `;
  }
}
