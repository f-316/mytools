import './styles/style.css';
import { TheContents } from './components/TheContents.ts';

TheContents.use();

document.querySelector<HTMLDivElement>('#app')!.innerHTML = /* HTML */ `
  <the-contents></the-contents>
`;
