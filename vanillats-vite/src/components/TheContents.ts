import { CustomElement } from '@/components/CustomElement';
import { CustomButton } from '@/components/CustomButton';

export class TheContents extends CustomElement {
  #button: CustomButton;

  constructor() {
    // コンストラクターでは常に super を最初に呼び出してください
    super(TheContents);

    // メンバの初期化
    this.#button = new CustomButton();

    // メンバのセットアップ
    this.#setup();

    // メンバのセットアップが終わってから呼び出して下さい
    this._setInnerHTML();
    this._setStyle();
  }

  /**
   * メンバのセットアップ
   */
  #setup() {
    this.#button.innerHTML = `
      <span slot="icon">A</span>
      <span slot="caption">B</span>
    `;
    this.#button.addEventListener('click', this.#onClickButton.bind(this));
  }

  #onClickButton(event: MouseEvent) {
    // const contents = this._shadow.querySelectorAll('.contents');
    // const customElems = this._shadow.querySelectorAll('custom-button');
    this.#button.backgroundColor = 'red';
  }

  /**
   * 内容をセットします。
   */
  _setInnerHTML() {
    this._contents.innerHTML = `
      <div class="contents">
      </div>
    `;
    this._contents.querySelector('.contents')!.append(this.#button);
  }

  /**
   * スタイルをセットします。
   */
  _setStyle() {
    const backgroundColor = this.getAttribute('background-color');
    this._style.textContent = `
      .contents {
        display: flex;
        align-items: center;
        background-color: ${backgroundColor};
      }
    `;
  }
}
