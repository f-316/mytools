import { CustomElement } from '@/components/CustomElement';
import { CustomButton, type CustomButtonEvent } from '@/components/CustomButton';

export class TheContents extends CustomElement {
  #buttons: CustomButton[] = [];

  constructor() {
    // コンストラクターでは常に super を最初に呼び出してください
    super(TheContents);

    // メンバの初期化
    [...Array(5)].forEach((_, index) => {
      this.#buttons.push(new CustomButton());
      this.#buttons[index].setAttribute('data-id', `${index}`);
    });

    // セットアップ
    this.#setup();

    // セットアップが終わってから呼び出して下さい
    this._setInnerHTML();
    this._setStyle();
  }

  /**
   * セットアップ
   */
  #setup() {
    this.#buttons.forEach((el) => {
      el.innerHTML = `
        <span slot="icon">A</span>
        <span slot="caption">B</span>
      `;
      el.addEventListener('#click', this.#onClickButton.bind(this) as EventListener);
    });
  }

  #onClickButton(event: CustomButtonEvent) {
    const btn = this.#buttons.find((el) => {
      return el === event.target;
    });

    if (btn) {
      if (btn!.backgroundColor !== 'red') {
        btn!.backgroundColor = 'red';
      } else {
        btn!.backgroundColor = 'unset';
      }
    }

    console.log(event!.detail);
    console.log(event!.detail?.name);
    console.log(event!.detail?.uid);
  }

  /**
   * 内容をセットします。
   */
  _setInnerHTML() {
    this._contents.innerHTML = /* HTML */ `
      <div class="contents"></div>
      <div class></div>
    `;

    this._contents.querySelector('.contents')!.append(...this.#buttons);
  }

  /**
   * スタイルをセットします。
   */
  _setStyle() {
    const backgroundColor = this.getAttribute('background-color');
    this._style.textContent = /* CSS */ `
      .contents {
        display: flex;
        align-items: center;
        background-color: ${backgroundColor};
      }
      .hoge {}
    `;
  }
}
