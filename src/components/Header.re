open ReactUtils;

[@react.component]
let make = (~currentRoute: Routes.t) => {
  let {UserProvider.user} = UserProvider.useUser();
  let isActive = route => currentRoute == route ? "active" : "";

  <nav className="navbar navbar-light">
    <div className="container">
      <Link className="navbar-brand" href="/"> "conduit"->s </Link>
      <ul className="nav navbar-nav pull-xs-right">
        <li className="nav-item">
          <Link className={"nav-link " ++ isActive(Home)} href="/">
            "Home"->s
          </Link>
        </li>
        {switch (user) {
         | LoggedIn({username}) =>
           <>
             <li className="nav-item">
               <Link
                 className={"nav-link " ++ isActive(CreateArticle)}
                 href="#/editor">
                 <i className="ion-compose" />
                 "New Post"->s
               </Link>
             </li>
             <li className="nav-item">
               <Link
                 className={"nav-link " ++ isActive(Settings)}
                 href="#/settings">
                 <i className="ion-gear-a" />
                 "Settings"->s
               </Link>
             </li>
             <li className="nav-item">
               <Link
                 className={
                   "nav-link "
                   ++ isActive(Profile(username, false))
                   ++ isActive(Profile(username, true))
                 }
                 href={"/#/profile/" ++ username}>
                 username->s
               </Link>
             </li>
           </>
         | Anonymous =>
           <>
             <li className="nav-item">
               <Link
                 className={"nav-link " ++ isActive(Login)} href="#/login">
                 "Sign in"->s
               </Link>
             </li>
             <li className="nav-item">
               <Link
                 className={"nav-link " ++ isActive(Register)}
                 href="#/register">
                 "Sign up"->s
               </Link>
             </li>
           </>
         }}
      </ul>
    </div>
  </nav>;
};