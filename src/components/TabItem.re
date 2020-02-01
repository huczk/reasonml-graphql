open ReactUtils;

[@react.component]
let make = (~active=false, ~onClick=ignore, ~text) => {
  <li className="nav-item">
    <Link href="" className={"nav-link " ++ (active ? "active" : "")} onClick>
      text->s
    </Link>
  </li>;
};