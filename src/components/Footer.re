open ReactUtils;

[@react.component]
let make = () => {
  <footer>
    <div className="container">
      <Link href="/" className="logo-font"> "conduit"->s </Link>
      <span className="attribution">
        "An interactive learning project from "->s
        <a href="https://thinkster.io"> "Thinkster"->s </a>
        ". Code & design licensed under MIT."->s
      </span>
    </div>
  </footer>;
};