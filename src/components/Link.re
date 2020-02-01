let handleClick = (href, event) =>
  if (!ReactEvent.Synthetic.defaultPrevented(event)) {
    ReactEvent.Synthetic.preventDefault(event);
    ReasonReact.Router.push(href);
  };

[@react.component]
let make = (~href, ~className="", ~children, ~onClick=handleClick(href)) =>
  <a href className onClick> children </a>;