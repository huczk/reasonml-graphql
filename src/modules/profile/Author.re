open ReactUtils;

[@react.component]
let make = (~username, ~image, ~createdAt) => {
  let href = "#/profile/" ++ username;

  <>
    <Link href> <img src=image /> </Link>
    <div className="info">
      <Link href className="author"> username->s </Link>
      <span className="date"> createdAt->s </span>
    </div>
  </>;
};