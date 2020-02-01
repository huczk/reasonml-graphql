open ReactUtils;

[@react.component]
let make = (~article) => {
  let isAuthor =
    UserProvider.useUser().user
    ->UserProvider.isLoggedUser(article##author##id);

  <div className="article-meta">
    <Author
      username=article##author##username
      image=article##author##image
      createdAt=article##preview##createdAt
    />
    {isAuthor
       ? <>
           <Link
             href={"#/editor/" ++ article##preview##slug}
             className="btn btn-outline-secondary btn-sm">
             <i className="ion-edit" />
             " Edit Article"->s
           </Link>
           <DeleteButton slug=article##preview##slug />
         </>
       : <>
           <FollowButton
             id=article##author##id
             username=article##author##username
             following=article##author##following
           />
           <FavoriteButton
             id=article##preview##id
             slug=article##preview##slug
             count=article##preview##favoritesCount
             favorited=article##preview##favorited
           />
         </>}
  </div>;
};