open ReactUtils;

[@react.component]
let make = (~comment, ~slug) => {
  let isAuthor =
    UserProvider.useUser().user
    ->UserProvider.isLoggedUser(comment##author##id);

  <div className="card">
    <div className="card-block">
      <p className="card-text"> {comment##body->s} </p>
    </div>
    <div className="card-footer">
      <Link
        href={"/#/profile/" ++ comment##author##username}
        className="comment-author">
        <img src=comment##author##image className="comment-author-img" />
      </Link>
      " "->s
      <Link
        href={"/#/profile/" ++ comment##author##username}
        className="comment-author">
        {comment##author##username->s}
      </Link>
      <span className="date-posted"> {comment##createdAt->s} </span>
      {isAuthor
         ? <span className="mod-options">
             <CommentDelete id=comment##id slug />
           </span>
         : React.null}
    </div>
  </div>;
};