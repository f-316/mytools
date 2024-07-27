import './styles/style.css';
import { TheContents } from './components/TheContents.ts';
import { CustomElement } from '@/components/CustomElement.ts';

TheContents.use();

document.querySelector<HTMLDivElement>('#app')!.innerHTML = /* HTML */ `
  <the-contents></the-contents>
`;

console.log(CustomElement.list);

class EventTest {
  m_name = '';
  m_div: HTMLElement | null = null;
  m_abortController: AbortController;
  constructor(name: string) {
    this.m_name = name;
    this.m_abortController = new AbortController();
    this.setup();
  }

  protected get abortSignal() {
    return this.m_abortController.signal;
  }

  protected get willRemove() {
    return { signal: this.abortSignal };
  }

  protected setup() {
    // document.body.addEventListener('click', this.onClick);
    const app = document.body.querySelector('#app');
    if (!app) return;
    this.m_div = document.createElement('div');
    console.log(`${this.m_name},setup`);
    this.m_div.innerHTML = /* HTML */ `<div>BUTTON</div>`;
    this.m_div.style.cssText = `
      height: 100px;
      width: 100px;
      border: 1px solid red;
    `;
    app.appendChild(this.m_div);
    if (!this.m_div) return;
    this.m_div.addEventListener('click', this.onClick);
    document.body.addEventListener('click', this.onClick);
    document.body.addEventListener('click', this.onClick2, this.willRemove);
    document.body.addEventListener(
      'click',
      (event: Event) => {
        console.log('arrow function');
        console.log(this.m_name);
        console.log(event);
        console.log(event.target);
      },
      this.willRemove
    );
    console.log('this.m_div.addEventListener');
  }

  dispose() {
    console.log(`${this.m_name},dispose`);

    this.m_div!.remove();
    document.body.removeEventListener('click', this.onClick);
    this.m_abortController.abort();
  }

  greet() {
    console.log('hello!');
  }

  protected onClick = (event: MouseEvent) => {
    console.log('onClick');
    console.log(this.m_name);
    console.log(event);
    console.log(event.target);
  };

  protected onClick2 = (event: MouseEvent) => {
    console.log('onClick2');
    console.log(this.m_name);
    console.log(event);
    console.log(event.target);
  };
}

let eventTest: EventTest | undefined = new EventTest('hoge');
eventTest.greet();
setTimeout(() => {
  console.log('delete');
  eventTest!.dispose();
  eventTest = undefined;
}, 3000);
